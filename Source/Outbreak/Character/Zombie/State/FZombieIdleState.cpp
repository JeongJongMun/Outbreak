#include "FZombieIdleState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

void FZombieIdleState::Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);
	
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

void FZombieIdleState::Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);
	
}