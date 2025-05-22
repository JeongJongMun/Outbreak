// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Core/TStateMachine.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "ZombieAI.generated.h"

struct FAIStimulus;
enum class EZombieState : uint8;

UCLASS()
class OUTBREAK_API AZombieAI : public AAIController
{
	GENERATED_BODY()

public:
	AZombieAI();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void InitializeStateMachine(class ACharacterZombie* InZombie);
	EZombieState GetCurrentState() const;
	
protected:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
public:
	TSharedPtr<TStateMachine<EZombieState, ACharacterPlayer>> StateMachine;

protected:
	TObjectPtr<class ACharacterZombie> OwnerZombie;
	TObjectPtr<UAIPerceptionComponent> AIPerception;
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
};