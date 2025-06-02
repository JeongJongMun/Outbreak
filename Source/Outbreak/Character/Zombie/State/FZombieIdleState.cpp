#include "FZombieIdleState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

void FZombieIdleState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);
	
	Owner->StopMovement();

	IdleTime = FMath::RandRange(MinIdleTime, MaxIdleTime);
	IdleTimer = 0.0f;
}

void FZombieIdleState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	
	IdleTimer += DeltaTime;

	if (IdleTimer >= IdleTime)
	{
		Owner->StateMachine->ChangeState(EZombieStateType::Wander);
	}
}

void FZombieIdleState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);
	
}