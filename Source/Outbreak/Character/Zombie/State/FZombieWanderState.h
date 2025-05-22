#pragma once
#include "FZombieStateBase.h"
#include "Outbreak/Util/Define.h"

enum class EZombieState : uint8;

class FZombieWanderState : public FZombieStateBase
{
public:
	FZombieWanderState(const TSharedPtr<TStateMachine<EZombieState, ACharacterPlayer>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
	: FZombieStateBase(InFsm, EZombieState::Wander, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer) override;

private:
	float WanderRadius = 500.0f;
	float WanderSpeed = 100.0f;
	float MinWanderTime = 5.0f;
	float MaxWanderTime = 15.0f;

	float WanderTimer = 0.0f;

	void StartWandering();
	bool FindRandomWanderLocation(FVector& OutLocation);
};
