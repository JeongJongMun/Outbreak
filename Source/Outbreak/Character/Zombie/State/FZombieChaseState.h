// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "FZombieBaseState.h"
#include "Outbreak/Util/Define.h"

class OUTBREAK_API FZombieChaseState : public FZombieBaseState
{
public:
	FZombieChaseState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, ACharacterZombie* InOwner);

	virtual void Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieStateType CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;

private:
	FDelegateHandle DelegateHandle;
};
