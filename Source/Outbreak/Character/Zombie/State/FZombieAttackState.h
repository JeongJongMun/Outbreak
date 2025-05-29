#pragma once

#include "CoreMinimal.h"
#include "FZombieBaseState.h"
#include "Outbreak/Util/Define.h"

class OUTBREAK_API FZombieAttackState : public FZombieBaseState
{
public:
	FZombieAttackState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieBaseState(InFsm, EZombieStateType::Attack, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> Context = nullptr) override;
	virtual void Execute(EZombieStateType CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> Context = nullptr) override;
};
