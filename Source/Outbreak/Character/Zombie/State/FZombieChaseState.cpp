// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieChaseState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

void FZombieChaseState::Enter(EZombieStateType PreviousState, const TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);

	if (!TargetPlayer)
		return;

	const TObjectPtr<UCharacterMovementComponent> MovementComp = Zombie->GetCharacterMovement();
	const FZombieData* ZombieData = Zombie->GetZombieData();
	
	if (Zombie && MovementComp)
	{
		if (ZombieData->bIsCanRun)
			MovementComp->MaxWalkSpeed = ZombieData->MaxRunSpeed;
		else
			MovementComp->MaxWalkSpeed = ZombieData->MaxWalkSpeed;
	}
	
	CurrentTargetPlayer = TargetPlayer;
	float AcceptanceRadius = ZombieData->AttackRange;

	DelegateHandle = Owner->GetPathFollowingComponent()->OnRequestFinished.AddLambda([this](FAIRequestID RequestID, const FPathFollowingResult& Result)
	{
		if (Result.IsSuccess())
		{
			Fsm->ChangeState(EZombieStateType::Attack, CurrentTargetPlayer);
		}
		else if (Result.IsFailure())
		{
			// Try to move again or switch to another state
		}
	});
	
	Owner->MoveToActor(CurrentTargetPlayer, AcceptanceRadius, true);

	FTimerHandle TimerHandle;
	Zombie->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AcceptanceRadius]()
	{
		DrawDebugSphere(Zombie->GetWorld(), Zombie->GetCharacterMovement()->GetActorLocation(), AcceptanceRadius, 12, FColor::Blue, false, 1.0f);
	}, 3.0f, true);
	
}

void FZombieChaseState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
}

void FZombieChaseState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);

	CurrentTargetPlayer = nullptr;
	Owner->GetPathFollowingComponent()->OnRequestFinished.Remove(DelegateHandle);
}