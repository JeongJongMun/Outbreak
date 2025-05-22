// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieChaseState.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

void FZombieChaseState::Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);

	if (TargetPlayer)
	{
		CurrentTargetPlayer = TargetPlayer;
		Owner->MoveToActor(TargetPlayer);
		Zombie->PlayAnimation(EZombieAnimationType::Run);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] No target to chase"), CURRENT_CONTEXT);
	}

	FZombieData* ZombieData = Zombie->GetZombieData();
	float AttackRange = ZombieData->AttackRange;
	FTimerHandle TimerHandle;
	Zombie->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AttackRange]()
	{
		DrawDebugSphere(Zombie->GetWorld(), Zombie->GetCharacterMovement()->GetActorLocation(), AttackRange, 12, FColor::Blue, false, 1.0f);
	}, 3.0f, true);
}

void FZombieChaseState::Execute(EZombieState CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	float Distance = FVector::Distance(Zombie->GetActorLocation(), CurrentTargetPlayer->GetActorLocation());
	FZombieData* ZombieData = Zombie->GetZombieData();
	float AttackRange = ZombieData->AttackRange;
	if (Distance <= AttackRange)
	{
		Fsm->ChangeState(EZombieState::Attack, CurrentTargetPlayer);
	}
	else if (Distance > AttackRange)
	{
		// Zombie->MoveToActor(CurrentTargetPlayer);
	}
	else
	{
		// Zombie->StopMovement();
	}
}

void FZombieChaseState::Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);
}
