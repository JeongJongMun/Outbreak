// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CharacterSpawnManager.h"
#include "CharacterFactory.generated.h"

class ACharacterBase;

UCLASS()
class OUTBREAK_API UCharacterFactory : public UObject
{
	GENERATED_BODY()

// --------------------
// Functions
// --------------------
public:
	UCharacterFactory();
	ACharacterBase* CreateCharacter(UWorld* InWorld, const FCharacterSpawnParam& InSpawnParam);
	
private:
	void InitializeFactoryMaps();
	TSubclassOf<ACharacterBase> GetCharacterClassFromType(const FCharacterSpawnParam& InSpawnParam);

// --------------------
// Variables
// --------------------
private:
	TMap<EZombieSubType, TSubclassOf<ACharacterBase>> ZombieClassMap;
	TMap<EPlayerType, TSubclassOf<ACharacterBase>> PlayerClassMap;

};