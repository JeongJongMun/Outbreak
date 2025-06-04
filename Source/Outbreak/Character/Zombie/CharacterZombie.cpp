// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterZombie.h"

#include "Outbreak/Util/MeshLoadHelper.h"
#include "State/FZombieIdleState.h"

ACharacterZombie::ACharacterZombie()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	PrimaryActorTick.bCanEverTick = true;
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

void ACharacterZombie::BeginPlay()
{
	Super::BeginPlay();
	
	ZombieAIController = CastChecked<AZombieAI>(GetController());
	ZombieAIController->InitializeStateMachine(this);
}

void ACharacterZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterZombie::InitializeZombieData(FZombieData* InData)
{
	ZombieData = *InData;
	CurrentHealth = ZombieData.MaxHealth;
	CurrentExtraHealth = 0;
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
	if (!ZombieAIController->StateMachine.IsValid())
	return;

	ZombieAIController->StateMachine->ChangeState(NewState, TargetPlayer);
}

void ACharacterZombie::Die()
{
	Super::Die();

	ChangeZombieState(EZombieStateType::Die);
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
	UE_LOG(LogTemp, Log, TEXT("%s's Attack Damage: %d, Final Damage: %d"), *GetName(), ZombieData.AttackDamage, FinalDamage);
	
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * ZombieData.AttackRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// TODO : Delete DebugLine
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		UE_LOG(LogTemp, Log, TEXT("1"));
		if (Hit.GetActor() && Hit.GetActor()->IsA(ACharacterPlayer::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("2"));
			if (ACharacterPlayer* Player = Cast<ACharacterPlayer>(Hit.GetActor()))
			{
				UE_LOG(LogTemp, Log, TEXT("3"));
				Player->TakeHitDamage(Hit, FinalDamage);
			}
		}
	}
	
	ChangeZombieState(EZombieStateType::Chase, ZombieAIController->CurrentTargetPlayer);
}
