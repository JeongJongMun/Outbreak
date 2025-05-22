// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FZombieStateBase.h"

class OUTBREAK_API FZombieChaseState : public FZombieStateBase
{
public:
	FZombieChaseState(const TSharedPtr<TStateMachine<EZombieState>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieStateBase(InFsm, EZombieState::Chase, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieState PreviousState) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState) override;

private:
	float ChaseAcceptanceRadius = 10000.0f;
};
