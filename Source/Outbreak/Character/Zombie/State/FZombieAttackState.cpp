#include "FZombieAttackState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

FZombieAttackState::FZombieAttackState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm, ACharacterZombie* InOwner): FZombieBaseState(InFsm, EZombieStateType::Attack, InOwner)
{
}

void FZombieAttackState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieBaseState::Enter(PreviousState, Context);
	
}

void FZombieAttackState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	FZombieBaseState::Execute(CurrentState, DeltaTime);
	
	RotateTowardsTarget(DeltaTime);
}

void FZombieAttackState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieBaseState::Exit(NextState, Context);
}
