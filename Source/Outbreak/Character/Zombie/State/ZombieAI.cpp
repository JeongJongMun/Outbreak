// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAI.h"

#include "FZombieAttackState.h"
#include "FZombieChaseState.h"
#include "FZombieDieState.h"
#include "FZombieIdleState.h"
#include "FZombieStateMachine.h"
#include "FZombieWanderState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

AZombieAI::AZombieAI()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AZombieAI::BeginPlay()
{
	Super::BeginPlay();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieAI::OnTargetPerceptionUpdated);
}

void AZombieAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (StateMachine.IsValid())
	{
		StateMachine->Execute(DeltaTime);
	}
}

void AZombieAI::InitializeZombieAI(ACharacterZombie* InZombie)
{
	Owner = InZombie;

	const auto* Data = Owner->GetZombieData();
	SightConfig->SightRadius = Data->SightRadius;
	SightConfig->LoseSightRadius = Data->LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = Data->PeripheralVisionAngleDegrees;
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 1000.0f;
	SightConfig->SetMaxAge(10.0f);

	AIPerception->ConfigureSense(*SightConfig);
	
	StateMachine = MakeShared<FZombieStateMachine>();
	StateMachine->AddState(EZombieStateType::Idle, MakeShared<FZombieIdleState>(StateMachine, Owner));
	StateMachine->AddState(EZombieStateType::Wander, MakeShared<FZombieWanderState>(StateMachine, Owner));
	// StateMachine->AddState(EZombieStateType::Alert, MakeShared<FZombieAlertState>(StateMachine, Owner));
	StateMachine->AddState(EZombieStateType::Chase, MakeShared<FZombieChaseState>(StateMachine, Owner));
	StateMachine->AddState(EZombieStateType::Attack, MakeShared<FZombieAttackState>(StateMachine, Owner));
	// StateMachine->AddState(EZombieStateType::Stun, MakeShared<FZombieStunState>(StateMachine, Owner));
	StateMachine->AddState(EZombieStateType::Die, MakeShared<FZombieDieState>(StateMachine, Owner));
	
	StateMachine->ChangeState(EZombieStateType::Idle);
}

EZombieStateType AZombieAI::GetCurrentState() const
{
	if (StateMachine.IsValid())
	{
		return StateMachine->GetCurrentState();
	}
	return EZombieStateType::None;
}

void AZombieAI::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	const TObjectPtr<ACharacterPlayer> TargetPlayer = Cast<ACharacterPlayer>(Actor);
	CurrentTargetPlayer = TargetPlayer;
	
	if (Stimulus.WasSuccessfullySensed())
	{
		if (GetCurrentState() == EZombieStateType::Idle || GetCurrentState() == EZombieStateType::Wander)
		{
			StateMachine->ChangeState(EZombieStateType::Chase, TargetPlayer);
		}
	}
	else
	{
		StateMachine->ChangeState(EZombieStateType::Wander);
	}
}
