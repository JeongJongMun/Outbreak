#pragma once
#include "FZombieBaseState.h"

class FZombieDieState : public FZombieBaseState
{
public:
	FZombieDieState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieBaseState(InFsm, EZombieStateType::Die, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieStateType CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override; 
};
