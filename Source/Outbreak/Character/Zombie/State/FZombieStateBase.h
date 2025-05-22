// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieAI.h"
#include "Outbreak/Character/CharacterBase.h"
#include "Outbreak/Core/TStateMachine.h"

class OUTBREAK_API FZombieStateBase : public TState<EZombieState, ACharacterPlayer>
{
public:
	using Super = FZombieStateBase;
	
	FZombieStateBase(const TSharedPtr<TStateMachine<EZombieState, ACharacterPlayer>>& InFsm, const EZombieState InStateKey,
		AZombieAI* InOwner, ACharacterZombie* InZombie)
			: TState(InFsm), Owner(InOwner), Zombie(InZombie)
	{
		StateKey = InStateKey;
	};
	virtual ~FZombieStateBase() override;
	virtual void Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;

protected:
	TObjectPtr<AZombieAI> Owner;
	TObjectPtr<ACharacterZombie> Zombie;
};
