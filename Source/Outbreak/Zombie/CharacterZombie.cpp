// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterZombie.h"
#include "State/FZombieIdleState.h"

ACharacterZombie::ACharacterZombie()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AZombieAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bIsAggressive = false;

	ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnim(TEXT("/Script/Engine.AnimSequence'/Game/Gruesome_Zombie_AnimSet/Animation/UE5/idle.idle'"));
	if (IdleAnim.Succeeded())
	{
		IdleAnimation = IdleAnim.Object;
		AnimationMap.Add(EZombieAnimationType::Idle, IdleAnimation);
	}
	ConstructorHelpers::FObjectFinder<UAnimSequence> WalkAnim(TEXT("/Script/Engine.AnimSequence'/Game/Gruesome_Zombie_AnimSet/Animation/UE5/move_walk_normal01.move_walk_normal01'"));
	if (WalkAnim.Succeeded())
	{
		WalkAnimation = WalkAnim.Object;
		AnimationMap.Add(EZombieAnimationType::Walk, WalkAnimation);
	}
	ConstructorHelpers::FObjectFinder<UAnimSequence> RunAnim(TEXT("/Script/Engine.AnimSequence'/Game/Gruesome_Zombie_AnimSet/Animation/UE5/move_offensive01_run_front.move_offensive01_run_front'"));
	if (RunAnim.Succeeded())
	{
		RunAnimation = RunAnim.Object;
		AnimationMap.Add(EZombieAnimationType::Run, RunAnimation);
	}
}

void ACharacterZombie::BeginPlay()
{
	Super::BeginPlay();
	
	AZombieAI* ZombieAIController = Cast<AZombieAI>(GetController());
	if (ZombieAIController)
	{
		ZombieAIController->InitializeStateMachine(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie AI Controller is not set!"));
	}
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
