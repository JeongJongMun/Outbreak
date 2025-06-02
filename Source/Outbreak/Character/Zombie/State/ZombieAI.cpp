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
	// Aggro Distance
	SightConfig->SightRadius = 800.f;
	SightConfig->LoseSightRadius = 850.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->SetMaxAge(5.f);

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

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		DrawDebugSphere(GetWorld(), OwnerZombie->GetCharacterMovement()->GetActorLocation(), SightConfig->SightRadius, 12, FColor::Red, false, 1.0f);
	}, 3.0f, true);
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
	StateMachine->AddState(EZombieStateType::Idle, MakeShared<FZombieIdleState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieStateType::Wander, MakeShared<FZombieWanderState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieStateType::Alert, MakeShared<FZombieAlertState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieStateType::Chase, MakeShared<FZombieChaseState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieStateType::Attack, MakeShared<FZombieAttackState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieStateType::Stun, MakeShared<FZombieStunState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieStateType::Die, MakeShared<FZombieDieState>(StateMachine, this, OwnerZombie));
	
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
