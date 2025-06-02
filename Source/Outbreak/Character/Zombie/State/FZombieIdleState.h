#pragma once
#include "FZombieBaseState.h"
#include "Outbreak/Util/Define.h"

enum class EZombieStateType : uint8;

class FZombieIdleState : public FZombieBaseState
{
public:
	FZombieIdleState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieBaseState(InFsm, EZombieStateType::Idle, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieStateType CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;

private:
	float MinIdleTime = 3.0f;
	float MaxIdleTime = 9.0f;
	
	float IdleTimer = 0.0f;
	float IdleTime = 0.0f;
};
