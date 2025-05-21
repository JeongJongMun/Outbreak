// Fill out your copyright notice in the Description page of Project Settings.

#include "OBGameMode.h"
#include "Outbreak/Character/CharacterPlayer.h"
#include "Outbreak/Zombie/ZombieSpawnManager.h"

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
