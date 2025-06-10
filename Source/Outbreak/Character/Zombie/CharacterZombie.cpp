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
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> ZombieAnimationMontage(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Zombie/Montage_Zombie.Montage_Zombie'"));
	if (ZombieAnimationMontage.Succeeded())
	{
		AnimMontage = ZombieAnimationMontage.Object;
	}

	MontageSectionNameMap.Add(EZombieStateType::Idle, IdleSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Wander, WanderSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseRunSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Attack, AttackSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Die, DieSectionName);
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

void ACharacterZombie::PlayAnimation(const EZombieStateType AnimType)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AnimMontage)
		return;

	// TODO : Section Name Manage
	const FName SectionPrefix = MontageSectionNameMap[AnimType];
	const FName SectionName = FName(*FString::Printf(TEXT("%s%d"), *SectionPrefix.ToString(), 0));

	if (AnimInstance->Montage_IsPlaying(AnimMontage))
	{
		const FName CurrentSection = AnimInstance->Montage_GetCurrentSection(AnimMontage);
		if (CurrentSection != NAME_None && CurrentSection != SectionName)
		{
			AnimInstance->Montage_Stop(0.5f, AnimMontage);
			AnimInstance->Montage_Play(AnimMontage);
			AnimInstance->Montage_JumpToSection(SectionName, AnimMontage); 
		}
	}
	else
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage); 
	}
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
	
	ChangeZombieState(EZombieStateType::Chase, ZombieAI->CurrentTargetPlayer);
}