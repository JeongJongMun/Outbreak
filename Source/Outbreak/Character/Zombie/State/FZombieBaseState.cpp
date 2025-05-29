// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieBaseState.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

FZombieBaseState::~FZombieBaseState()
{
}

void FZombieBaseState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Zombie->PlayAnimation(StateKey);
	UE_LOG(LogTemp, Log, TEXT("[%s] Entering state: %s"), CURRENT_CONTEXT, *UEnum::GetValueAsString(StateKey));
}

void FZombieBaseState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
}

void FZombieBaseState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Exiting state: %s"), CURRENT_CONTEXT, *UEnum::GetValueAsString(StateKey));
}