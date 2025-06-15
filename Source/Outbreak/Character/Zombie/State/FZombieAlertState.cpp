#include "FZombieAlertState.h"

#include "Outbreak/Character/Zombie/CharacterZombie.h"

FZombieAlertState::FZombieAlertState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm,
                                     ACharacterZombie* InOwner) : FZombieBaseState(InFsm, EZombieStateType::Alert, InOwner)
{
}

void FZombieAlertState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieBaseState::Enter(PreviousState, Context);
}

void FZombieAlertState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	FZombieBaseState::Execute(CurrentState, DeltaTime);

	RotateTowardsTarget(DeltaTime);
	
	Timer += DeltaTime;
	if (Timer >= CurrentAnimationLength)
	{
		const TObjectPtr<AZombieAI> ZombieAI = Owner->GetZombieAI();
		ZombieAI->StateMachine->ChangeState(EZombieStateType::Chase, CurrentTargetPlayer);
	}
}

void FZombieAlertState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieBaseState::Exit(NextState, Context);
	Timer = 0.0f;
}
