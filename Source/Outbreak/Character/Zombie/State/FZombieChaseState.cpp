// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieChaseState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

void FZombieChaseState::Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);

	if (TargetPlayer)
	{
		Owner->MoveToActor(TargetPlayer);
		Zombie->PlayAnimation(EZombieAnimationType::Run);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] No target to chase"), CURRENT_CONTEXT);
	}
}

void FZombieChaseState::Execute(EZombieState CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	
}

void FZombieChaseState::Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);
}
