// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterZombie.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Outbreak/Game/Framework/OutBreakGameState.h"
#include "Outbreak/Game/Framework/OutBreakPlayerState.h"
#include "Outbreak/Util/MeshLoadHelper.h"

ACharacterZombie::ACharacterZombie()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterType = ECharacterType::Zombie;
	AIControllerClass = AZombieAIComponent::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ZombieMegaBundle/Mesh_UE5/UE5_Skeleton/Body/SKM_Body_Afro_UE5.SKM_Body_Afro_UE5'"));
	if (DefaultMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DefaultMesh.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	
	TMap<EZombieAnimationType, FString> MontagePaths = {
		{EZombieAnimationType::Idle, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Idle.MT_Zombie_Idle'")},
		{EZombieAnimationType::Wander, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Wander.MT_Zombie_Wander'")},
		{EZombieAnimationType::Alert, TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/MT_Zombie_Alert.MT_Zombie_Alert'")},
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

void ACharacterZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterZombie, ZombieData);
}

void ACharacterZombie::InitCharacterData()
{
	Super::InitCharacterData();
	
	if (HasAuthority())
	{
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
		if (!Data)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] No Zombie data found for type: %d"), CURRENT_CONTEXT, (int32)ZombieSubType);
			return;
		}
		ZombieData = *Data;
		
	}
	
	CurrentHealth = ZombieData.MaxHealth;
	CurrentExtraHealth = 0;
}

void ACharacterZombie::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		ZombieAI = Cast<AZombieAIComponent>(GetController());
		if (!ZombieAI)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] ZombieAI is null!"), CURRENT_CONTEXT);
			return;
		}
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
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(false);
	GetCapsuleComponent()->BodyInstance.bLockXRotation = true;
	GetCapsuleComponent()->BodyInstance.bLockYRotation = true;
	GetCapsuleComponent()->BodyInstance.bLockZRotation = false;
    
	GetCapsuleComponent()->BodyInstance.LinearDamping = 1.0f;
	GetCapsuleComponent()->BodyInstance.AngularDamping = 10.0f;
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

	MovementComp->JumpZVelocity = 0.0f;
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
			const int32 SectionIndex = AnimMontage->GetSectionIndex(RandomSection);
			CurrentAnimationSectionLength = AnimMontage->GetSectionLength(SectionIndex);
			
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
	if (HasAuthority() && ZombieAI->StateMachine.IsValid())
	{
		ZombieAI->StateMachine->ChangeState(NewState, TargetPlayer);
	}
}

void ACharacterZombie::OnRep_Die()
{
	Super::OnRep_Die();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ChangeZombieState(EZombieStateType::Die);
	if (HasAuthority())
	{
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
}

void ACharacterZombie::StartMoveSoundTimer()
{
	Super::StartMoveSoundTimer();
	
	GetWorld()->GetTimerManager().SetTimer(
		MoveSoundTimerHandle,
		this,
		&ACharacterZombie::Multicast_PlayMoveSound,
		0.3f,
		true
	);	
}

void ACharacterZombie::Multicast_PlayMoveSound_Implementation()
{
	const int32 RandomIndex = FMath::RandRange(0, SprintSounds.Num() - 1);
	UGameplayStatics::PlaySoundAtLocation(this, SprintSounds[RandomIndex], GetActorLocation(), 0.3f);
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
	if (!HasAuthority())
		return;
	
	// TODO : 자동으로 공격 데미지 계산하는 구조로 개선
	const int32 FinalDamage = static_cast<int32>(ZombieData.AttackDamage * AttackDamageMultiplier);
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * ZombieData.AttackRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->IsA(ACharacterPlayer::StaticClass()))
		{
			UGameplayStatics::ApplyDamage(HitActor, FinalDamage, GetController(), this, UDamageType::StaticClass());
		}
	}

	const TObjectPtr<ACharacterPlayer> Target = ZombieAI->GetCurrentTargetCharacter();
	ChangeZombieState(EZombieStateType::Chase, Target);
}