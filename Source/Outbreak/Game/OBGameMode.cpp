// Fill out your copyright notice in the Description page of Project Settings.

#include "OBGameMode.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Character/Zombie/ZombieSpawnManager.h"

AOBGameMode::AOBGameMode()
{
    DefaultPawnClass = ACharacterPlayer::StaticClass();
}

void AOBGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AZombieSpawnManager* SpawnManager = GetWorld()->SpawnActor<AZombieSpawnManager>(AZombieSpawnManager::StaticClass(), SpawnParams);
}
