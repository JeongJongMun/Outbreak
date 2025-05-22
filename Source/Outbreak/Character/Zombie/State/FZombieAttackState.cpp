#include "FZombieAttackState.h"

#include "Outbreak/Character/Zombie/CharacterZombie.h"

void FZombieAttackState::Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieStateBase::Enter(PreviousState, Context);

	Zombie->PlayAnimation(EZombieAnimationType::Attack, false);
}

void FZombieAttackState::Execute(EZombieState CurrentState, float DeltaTime)
{
	FZombieStateBase::Execute(CurrentState, DeltaTime);
}

void FZombieAttackState::Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> Context)
{
	FZombieStateBase::Exit(NextState, Context);
}
