// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Outbreak/Util/Define.h"
#include "ZombieSpawnManager.generated.h"

UCLASS()
class OUTBREAK_API AZombieSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AZombieSpawnManager();

	UPROPERTY(EditDefaultsOnly, Category = "Zombie Spawning")
	TObjectPtr<class UZombieFactory> ZombieFactory;
    
	UPROPERTY(EditDefaultsOnly, Category = "Zombie Spawning")
	TObjectPtr<UDataTable> ZombieDataTable;
	
	UPROPERTY(EditInstanceOnly, Category = "Zombie Spawning")
	TArray<AActor*> SpawnPoints;
	
protected:
	virtual void BeginPlay() override;

public:	
	FZombieData* GetZombieData(EZombieSubType InZombieSubType) const;
	void SpawnZombie(EZombieSubType InZombieSubType,
		int Count = 1,
		const FVector& SpawnLocation = FVector::ZeroVector + FVector(0, 0, 100),
		const FRotator& SpawnRotation = FRotator::ZeroRotator);
};