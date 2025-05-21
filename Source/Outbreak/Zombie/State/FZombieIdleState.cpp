#include "FZombieIdleState.h"

void FZombieIdleState::Enter(EZombieState PreviousState)
{
	Super::Enter(PreviousState);
	
	Owner->StopMovement();

	IdleTime = FMath::RandRange(MinIdleTime, MaxIdleTime);
	IdleTimer = 0.0f;

	Zombie->PlayAnimation(EZombieAnimationType::Idle);
}

void FZombieIdleState::Execute(EZombieState CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	
	IdleTimer += DeltaTime;

	if (IdleTimer >= IdleTime)
	{
		Owner->StateMachine->ChangeState(EZombieState::Wander);
		return;
	}

	// TODO : Player Detection Logic
}

void FZombieIdleState::Exit(EZombieState NextState)
{
	Super::Exit(NextState);
	
}