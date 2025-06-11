// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieBaseState.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

FZombieBaseState::FZombieBaseState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm,
	const EZombieStateType InStateKey, ACharacterZombie* InOwner): TState(InFsm, InStateKey)
{
	Owner = InOwner;
}

void FZombieBaseState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Owner->PlayAnimation(StateKey);
	if (TargetPlayer)
	{
		CurrentTargetPlayer = TargetPlayer;
	}
}

void FZombieBaseState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
}

void FZombieBaseState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	CurrentTargetPlayer = nullptr;
}