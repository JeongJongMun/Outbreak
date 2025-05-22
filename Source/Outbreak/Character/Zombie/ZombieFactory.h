// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CharacterZombie.h"
#include "ZombieFactory.generated.h"

UCLASS()
class OUTBREAK_API UZombieFactory : public UObject
{
	GENERATED_BODY()

public:
	ACharacterZombie* CreateZombie(UWorld* World,
		EZombieSubType ZombieSubType,
		const FVector& SpawnLocation,
		const FRotator& SpawnRotation);
	
private:
	TSubclassOf<ACharacterZombie> GetZombieClassFromSubType(EZombieSubType ZombieSubType) const;

protected:
private:
};
