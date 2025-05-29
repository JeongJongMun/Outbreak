#include "FZombieDieState.h"

#include "Components/CapsuleComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"

void FZombieDieState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	FZombieBaseState::Enter(PreviousState, TargetPlayer);
	
	Zombie->GetController()->StopMovement();
	Zombie->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Zombie->SetLifeSpan(10.0f);
	
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
