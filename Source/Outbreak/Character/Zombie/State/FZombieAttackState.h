#pragma once

#include "CoreMinimal.h"
#include "FZombieStateBase.h"
#include "Outbreak/Util/Define.h"
class OUTBREAK_API FZombieAttackState : public FZombieStateBase
{
public:
	FZombieAttackState(const TSharedPtr<TStateMachine<EZombieState, ACharacterPlayer>>& InFsm, AZombieAI* InOwner, ACharacterZombie* InCharacter)
		: FZombieStateBase(InFsm, EZombieState::Attack, InOwner, InCharacter)
	{
	}

	virtual void Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> Context = nullptr) override;
	virtual void Execute(EZombieState CurrentState, float DeltaTime) override;
	virtual void Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> Context = nullptr) override;
	
};
