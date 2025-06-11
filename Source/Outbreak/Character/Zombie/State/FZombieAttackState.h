#pragma once

#include "CoreMinimal.h"
#include "FZombieBaseState.h"
#include "Outbreak/Util/Define.h"

class OUTBREAK_API FZombieAttackState : public FZombieBaseState
{
public:
	FZombieAttackState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, ACharacterZombie* InOwner);

	virtual void Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> Context = nullptr) override;
	virtual void Execute(EZombieStateType CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> Context = nullptr) override;

private:
	void RotateTowardsTarget(float DeltaTime) const;


private:
	float RotationSpeed = 180.0f;
};
