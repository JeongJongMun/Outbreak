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

void FZombieAttackState::RotateTowardsTarget(const float DeltaTime) const
{
	if (Owner && CurrentTargetPlayer)
	{
		const FVector TargetLocation = CurrentTargetPlayer->GetActorLocation();
		const FVector OwnerLocation = Owner->GetActorLocation();
        
		FVector Direction = TargetLocation - OwnerLocation;
		Direction.Z = 0.0f;
		Direction.Normalize();
        
		if (!Direction.IsNearlyZero())
		{
			const FRotator TargetRotation = Direction.Rotation();
			const FRotator CurrentRotation = Owner->GetActorRotation();
            
			const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
            
			Owner->SetActorRotation(NewRotation);
		}
	}
}
