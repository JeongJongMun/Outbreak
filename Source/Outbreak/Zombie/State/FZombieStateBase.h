// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieAI.h"
#include "Outbreak/Character/CharacterPlayer.h"
#include "Outbreak/Core/TStateMachine.h"
#include "Outbreak/Zombie/CharacterZombie.h"

class OUTBREAK_API FZombieStateBase : public TState<EZombieState>
{
public:
	using Super = FZombieStateBase;
	
	FZombieStateBase(const TSharedPtr<TStateMachine<EZombieState>>& InFsm, const EZombieState InStateKey,
		AZombieAI* InOwner, ACharacterZombie* InZombie)
			: TState(InFsm), Owner(InOwner), Zombie(InZombie)
	{
		StateKey = InStateKey;
	};
	virtual ~FZombieStateBase() override;
	virtual void Enter(EZombieState PreviousState) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState) override;
	

protected:
	bool FindTargetPlayer();
	AZombieAI* Owner;
	ACharacterZombie* Zombie;
	TObjectPtr<ACharacterBase> CurrentTarget;

private:
	float AggroDistance = 10000.0f;
	float NearestDistanceSq = TNumericLimits<float>::Max();
};