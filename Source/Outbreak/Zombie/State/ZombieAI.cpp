// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAI.h"

#include "FZombieChaseState.h"
#include "FZombieIdleState.h"
#include "FZombieStateMachine.h"
#include "FZombieWanderState.h"
#include "Outbreak/Util/Define.h"

AZombieAI::AZombieAI()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentTarget = nullptr;
}

void AZombieAI::BeginPlay()
{
	Super::BeginPlay();

}

void AZombieAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine.IsValid())
	{
		StateMachine->Execute(DeltaTime);
	}
}

void AZombieAI::InitializeStateMachine(ACharacterZombie* InZombie)
{
	OwnerZombie = InZombie;
	
	StateMachine = MakeShared<FZombieStateMachine>();
	StateMachine->AddState(EZombieState::Idle, MakeShared<FZombieIdleState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieState::Wander, MakeShared<FZombieWanderState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Alert, MakeShared<FZombieAlertState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieState::Chase, MakeShared<FZombieChaseState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Attack, MakeShared<FZombieAttackState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Stun, MakeShared<FZombieStunState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Die, MakeShared<FZombieDieState>(StateMachine, this, OwnerZombie));
	
	StateMachine->ChangeState(EZombieState::Idle);
}

void AZombieAI::ChangeState(EZombieState NewState)
{
	if (StateMachine.IsValid())
	{
		StateMachine->ChangeState(NewState);
	}
}

EZombieState AZombieAI::GetCurrentState() const
{
	if (StateMachine.IsValid())
	{
		return StateMachine->GetCurrentState();
	}
	return EZombieState::None;
}
