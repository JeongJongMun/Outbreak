// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Outbreak/Core/TStateMachine.h"
#include "ZombieAI.generated.h"

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
	void ChangeState(EZombieState NewState);
	EZombieState GetCurrentState() const;
	
public:
	TSharedPtr<TStateMachine<EZombieState>> StateMachine;

protected:
	TObjectPtr<ACharacterZombie> OwnerZombie;
	AActor* CurrentTarget;
};