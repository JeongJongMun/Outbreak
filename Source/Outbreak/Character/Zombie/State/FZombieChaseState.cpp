// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieChaseState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

void FZombieChaseState::Enter(EZombieState PreviousState)
{
	Super::Enter(PreviousState);
}

void FZombieChaseState::Execute(EZombieState CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);

	if (CurrentTarget)
	{
		Owner->MoveToActor(CurrentTarget, ChaseAcceptanceRadius);
		Zombie->PlayAnimation(EZombieAnimationType::Run);
	}
}

void FZombieChaseState::Exit(EZombieState NextState)
{
	Super::Exit(NextState);
}
