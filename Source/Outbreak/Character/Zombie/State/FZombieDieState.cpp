#include "FZombieDieState.h"

#include "Components/CapsuleComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

FZombieDieState::FZombieDieState(const TSharedPtr<TStateMachine<EZombieStateType, ACharacterPlayer>>& InFsm,
	ACharacterZombie* InOwner): FZombieBaseState(InFsm, EZombieStateType::Die, InOwner)
{
}

void FZombieDieState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	FZombieBaseState::Enter(PreviousState, TargetPlayer);
	
	Owner->GetController()->StopMovement();
	Owner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Owner->SetLifeSpan(10.0f);
	
	// TODO : Score, Exp
}

void FZombieDieState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	FZombieBaseState::Execute(CurrentState, DeltaTime);
}

void FZombieDieState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	FZombieBaseState::Exit(NextState, TargetPlayer);
	
}
