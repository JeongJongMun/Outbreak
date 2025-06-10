// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FZombieStateMachine.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "ZombieAI.generated.h"

struct FAIStimulus;
enum class EZombieStateType : uint8;

UCLASS()
class OUTBREAK_API AZombieAI : public AAIController
{
	GENERATED_BODY()

public:
	AZombieAI();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	void InitializeZombieAI(class ACharacterZombie* InZombie);
	EZombieStateType GetCurrentState() const;
	
protected:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
public:
	TSharedPtr<FZombieStateMachine> StateMachine;
	TObjectPtr<ACharacterPlayer> CurrentTargetPlayer;

protected:
	TObjectPtr<class ACharacterZombie> OwnerZombie;
	TObjectPtr<UAIPerceptionComponent> AIPerception;
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	FGenericTeamId TeamId = 1;
};