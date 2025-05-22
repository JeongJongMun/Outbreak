#pragma once
#include "FZombieStateBase.h"
#include "Outbreak/Util/Define.h"

enum class EZombieState : uint8;

class FZombieIdleState : public FZombieStateBase
{
public:
	FZombieIdleState(const TSharedPtr<TStateMachine<EZombieState>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieStateBase(InFsm, EZombieState::Idle, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieState PreviousState) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState) override;

private:
	float MinIdleTime = 3.0f;
	float MaxIdleTime = 9.0f;
	
	float IdleTimer = 0.0f;
	float IdleTime = 0.0f;
};
