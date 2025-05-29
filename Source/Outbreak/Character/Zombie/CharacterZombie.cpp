// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterZombie.h"
#include "State/FZombieIdleState.h"

ACharacterZombie::ACharacterZombie()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AZombieAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bIsAggressive = false;

	ConstructorHelpers::FObjectFinder<UAnimMontage> ZombieAnimationMontage(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Montage_Zombie.Montage_Zombie'"));
	if (ZombieAnimationMontage.Succeeded())
	{
		AnimMontage = ZombieAnimationMontage.Object;
		CurrentSection = IdleSectionName;
	}

	MontageSectionNameMap.Add(EZombieStateType::Idle, IdleSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Wander, WanderSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseSectionName);
	MontageSectionNameMap.Add(EZombieStateType::Attack, AttackSectionName);
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

void ACharacterZombie::PlayAnimation(EZombieStateType AnimType)
{
	if (!AnimMontage)
		return;

	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;
	
	const FName SectionPrefix = MontageSectionNameMap[AnimType];
	const FName SectionName = FName(*FString::Printf(TEXT("%s%d"), *SectionPrefix.ToString(), 0));
	if (AnimInstance->Montage_IsPlaying(AnimMontage))
	{
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
	else
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
}

void ACharacterZombie::ChangeZombieState(EZombieStateType NewState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	if (!ZombieAIController->StateMachine.IsValid())
	return;

	ZombieAIController->StateMachine->ChangeState(NewState, TargetPlayer);
}

void ACharacterZombie::OnAttackEnd()
{
	ChangeZombieState(EZombieStateType::Chase, ZombieAIController->CurrentTargetPlayer);
}
