// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "FZombieStateBase.h"
#include "Outbreak/Util/Define.h"

class OUTBREAK_API FZombieChaseState : public FZombieStateBase
{
public:
	FZombieChaseState(const TSharedPtr<TStateMachine<EZombieState, ACharacterPlayer>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieStateBase(InFsm, EZombieState::Chase, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;

private:
	TObjectPtr<ACharacterPlayer> CurrentTargetPlayer;
};
