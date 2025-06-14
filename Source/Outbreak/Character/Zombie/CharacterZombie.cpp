// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterZombie.h"
#include "CharacterSpawnManager.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Outbreak/Game/OutBreakGameState.h"
#include "Outbreak/Game/OutBreakPlayerState.h"
#include "Outbreak/Util/MeshLoadHelper.h"
#include "State/FZombieIdleState.h"

ACharacterZombie::ACharacterZombie()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterType = ECharacterType::Zombie;
	AIControllerClass = AZombieAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TMap<EZombieAnimationType, FString> MontagePaths = {
		{EZombieAnimationType::Idle, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Idle.MT_Zombie_Idle'")},
		{EZombieAnimationType::Wander, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Wander.MT_Zombie_Wander'")},
		{EZombieAnimationType::ChaseWalk, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Chase_Walk.MT_Zombie_Chase_Walk'")},
		{EZombieAnimationType::ChaseRun, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Chase_Run.MT_Zombie_Chase_Run'")},
		{EZombieAnimationType::Attack, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Attack.MT_Zombie_Attack'")},
		{EZombieAnimationType::Die, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Die.MT_Zombie_Die'")},
	};

	for (const auto& [Type, Path] : MontagePaths)
	{
		if (ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(*Path); MontageFinder.Succeeded())
		{
			AnimMontageMap.Add(Type, MontageFinder.Object);
		}
	}
}

void ACharacterZombie::InitCharacterData()
{
	Super::InitCharacterData();

	const AOutBreakGameState* GameState = Cast<AOutBreakGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] GameState is null!"), CURRENT_CONTEXT);
		return;
	}
	ACharacterSpawnManager* SpawnManager = GameState->GetSpawnManager();
	if (!SpawnManager)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] SpawnManager is null!"), CURRENT_CONTEXT);
		return;
	}
	const FZombieData* Data = SpawnManager->GetZombieData(ZombieSubType);
	ZombieData = *Data;
	CurrentHealth = ZombieData.MaxHealth;
	CurrentExtraHealth = 0;
}

void ACharacterZombie::BeginPlay()
{
	Super::BeginPlay();

	ZombieAI = Cast<AZombieAI>(GetController());
	if (ZombieAI)
	{
		ZombieAI->InitializeZombieAI(this);
	}
}

void ACharacterZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterZombie::SetupCollision()
{
	Super::SetupCollision();

	GetCapsuleComponent()->InitCapsuleSize(10.f, 96.0f);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ACharacterZombie::SetupMovement()
{
	Super::SetupMovement();

	auto* MovementComp = GetCharacterMovement();
	MovementComp->AvoidanceConsiderationRadius = 500.0f;
	MovementComp->SetAvoidanceEnabled(true);
	MovementComp->SetRVOAvoidanceWeight(0.3f);
	MovementComp->SetAvoidanceGroup(static_cast<int32>(EAvoidanceGroupType::Zombie));
	MovementComp->SetGroupsToAvoid(static_cast<int32>(EAvoidanceGroupType::Zombie));
	MovementComp->SetGroupsToIgnore(static_cast<int32>(EAvoidanceGroupType::Player));
}

void ACharacterZombie::PlayAnimation(const EZombieStateType InStateType)
{
	const EZombieAnimationType AnimationType = GetZombieAnimationTypeFromState(InStateType);

	if (const TObjectPtr<UAnimMontage> AnimMontage = AnimMontageMap.FindRef(AnimationType))
	{
		TArray<FName>& SectionNames = AnimSectionMap.FindOrAdd(AnimationType);
		if (SectionNames.IsEmpty())
		{
			for (const auto& Section : AnimMontage->CompositeSections)
			{
				SectionNames.Add(Section.SectionName);
			}
		}

		if (SectionNames.Num() > 0)
		{
			const FName RandomSection = SectionNames[FMath::RandRange(0, SectionNames.Num() - 1)];
			PlayAnimMontage(AnimMontage, 1.0f, RandomSection);
		}
	}
}

void ACharacterZombie::Multicast_PlayAnimation_Implementation(EZombieStateType InStateType)
{
	PlayAnimation(InStateType);
}

void ACharacterZombie::ChangeZombieState(const EZombieStateType NewState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	if (ZombieAI->StateMachine.IsValid())
	{
		ZombieAI->StateMachine->ChangeState(NewState, TargetPlayer);
	}
}

void ACharacterZombie::Die()
{
	Super::Die();

	ChangeZombieState(EZombieStateType::Die);

	if (AController* Killer = LastDamagePlayer)
	{
		if (AOutBreakPlayerState* PS = Cast<AOutBreakPlayerState>(Killer->PlayerState))
		{
			PS->AddZombieKill();
		}
	}
	if (AOutBreakGameState* GS = GetWorld()->GetGameState<AOutBreakGameState>())
	{
		GS->AddTotalZombieKill();
	}
}

float ACharacterZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	LastDamagePlayer = EventInstigator; // Save Last Instigator
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

EZombieAnimationType ACharacterZombie::GetZombieAnimationTypeFromState(const EZombieStateType InStateType)
{
	EZombieAnimationType AnimType = EZombieAnimationType::None;
	if (InStateType == EZombieStateType::Chase)
	{
		const uint8 bIsRunning = GetZombieData()->bIsCanRun;
		AnimType = bIsRunning ? EZombieAnimationType::ChaseRun : EZombieAnimationType::ChaseWalk;
	}

	switch (InStateType)
	{
	case EZombieStateType::Idle:
		AnimType = EZombieAnimationType::Idle;
		break;
	case EZombieStateType::Wander:
		AnimType = EZombieAnimationType::Wander;
		break;
	case EZombieStateType::Alert:
		AnimType = EZombieAnimationType::Alert;
		break;
	case EZombieStateType::Chase:
		break;
	case EZombieStateType::Attack:
		AnimType = EZombieAnimationType::Attack;
		break;
	case EZombieStateType::Stun:
		AnimType = EZombieAnimationType::Stun;
		break;
	case EZombieStateType::Die:
		AnimType = EZombieAnimationType::Die;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("[%s] Invalid Zombie State Type: %s"), CURRENT_CONTEXT, *UEnum::GetValueAsString(InStateType));
		return EZombieAnimationType::None;
	}

	return AnimType;
}

void ACharacterZombie::SetMesh(const ECharacterBodyType MeshType)
{
	const FString MeshTypeString = MeshLoadHelper::ZombieMeshTypeToString(MeshType);
	
	// TODO : Zombie Mesh Data Manage
	int NormalMesh = 20;
	int MuscleMesh = 6;
	int FatMesh = 6;

	int MeshCount = 0;
	switch (MeshType)
	{
		case ECharacterBodyType::Normal:
			MeshCount = NormalMesh;
			break;
		case ECharacterBodyType::Muscle:
			MeshCount = MuscleMesh;
			break;
		case ECharacterBodyType::Fat:
			MeshCount = FatMesh;
			break;
	}

	if (const TObjectPtr<USkeletalMesh> ZombieMesh = MeshLoadHelper::GetRandomZombieMesh(BaseMeshRef, BaseMeshName, MeshTypeString, MeshCount))
	{
		GetMesh()->SetSkeletalMesh(ZombieMesh);
	}
}


void ACharacterZombie::OnAttackEnd()
{
	// TODO : 자동으로 공격 데미지 계산하는 구조로 개선
	const int32 FinalDamage = static_cast<int32>(ZombieData.AttackDamage * AttackDamageMultiplier);
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * ZombieData.AttackRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// TODO : Delete DebugLine
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->IsA(ACharacterPlayer::StaticClass()))
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				FinalDamage,
				GetActorForwardVector(),
				Hit,
				GetController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}

	ChangeZombieState(EZombieStateType::Chase, ZombieAI->GetCurrentTargetCharacter());
}