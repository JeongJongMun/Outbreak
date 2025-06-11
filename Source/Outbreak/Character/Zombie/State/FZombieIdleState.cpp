#include "FZombieIdleState.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

FZombieIdleState::FZombieIdleState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm,
	ACharacterZombie* InOwner): FZombieBaseState(InFsm, EZombieStateType::Idle, InOwner)
{
}

void FZombieIdleState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);
	
	Owner->GetZombieAI()->StopMovement();

	IdleTime = FMath::RandRange(MinIdleTime, MaxIdleTime);
	IdleTimer = 0.0f;
}

void FZombieIdleState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	
	IdleTimer += DeltaTime;

	if (IdleTimer >= IdleTime)
	{
		const TObjectPtr<AZombieAI> ZombieAI = Owner->GetZombieAI();
		ZombieAI->StateMachine->ChangeState(EZombieStateType::Wander);
	}
}

void FZombieIdleState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);
	
}