// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAI.h"

#include "FZombieAttackState.h"
#include "FZombieChaseState.h"
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
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

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
	StateMachine->AddState(EZombieState::Idle, MakeShared<FZombieIdleState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieState::Wander, MakeShared<FZombieWanderState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Alert, MakeShared<FZombieAlertState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieState::Chase, MakeShared<FZombieChaseState>(StateMachine, this, OwnerZombie));
	StateMachine->AddState(EZombieState::Attack, MakeShared<FZombieAttackState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Stun, MakeShared<FZombieStunState>(StateMachine, this, OwnerZombie));
	// StateMachine->AddState(EZombieState::Die, MakeShared<FZombieDieState>(StateMachine, this, OwnerZombie));
	
	StateMachine->ChangeState(EZombieState::Idle);
}

EZombieState AZombieAI::GetCurrentState() const
{
	if (StateMachine.IsValid())
	{
		return StateMachine->GetCurrentState();
	}
	return EZombieState::None;
}

void AZombieAI::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	const TObjectPtr<ACharacterPlayer> TargetPlayer = Cast<ACharacterPlayer>(Actor);
	if (Stimulus.WasSuccessfullySensed())
	{
		StateMachine->ChangeState(EZombieState::Chase, TargetPlayer);
		UE_LOG(LogTemp, Warning, TEXT("%s의 시야에서 플레이어 감지됨: %s"), *OwnerZombie->GetName(), *Actor->GetName());
	}
	else
	{
		StateMachine->ChangeState(EZombieState::Wander, TargetPlayer);
		UE_LOG(LogTemp, Warning, TEXT("%s의 시야에서 플레이어 사라짐: %s"), *OwnerZombie->GetName(), *Actor->GetName());
	}
}
