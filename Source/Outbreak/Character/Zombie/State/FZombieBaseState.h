// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieAI.h"
#include "Outbreak/Core/TStateMachine.h"

class OUTBREAK_API FZombieBaseState : public TState<EZombieStateType, ACharacterPlayer>
{
public:
	using Super = FZombieBaseState;
	
	FZombieBaseState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, const EZombieStateType InStateKey,
		AZombieAI* InOwner, ACharacterZombie* InZombie)
			: TState(InFsm), Owner(InOwner), Zombie(InZombie)
	{
		StateKey = InStateKey;
	};
	virtual ~FZombieBaseState() override;
	virtual void Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieStateType CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;

protected:
	TObjectPtr<AZombieAI> Owner;
	TObjectPtr<ACharacterZombie> Zombie;
};
