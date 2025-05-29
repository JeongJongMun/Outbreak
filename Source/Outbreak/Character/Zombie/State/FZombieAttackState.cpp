#include "FZombieAttackState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

void FZombieAttackState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieBaseState::Enter(PreviousState, Context);
	
}

void FZombieAttackState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	FZombieBaseState::Execute(CurrentState, DeltaTime);
}

void FZombieAttackState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieBaseState::Exit(NextState, Context);
}