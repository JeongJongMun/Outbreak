// Fill out your copyright notice in the Description page of Project Settings.

#include "OBGameMode.h"

#include "OutBreakPlayerState.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Character/Zombie/CharacterSpawnManager.h"

AOBGameMode::AOBGameMode()
{
    DefaultPawnClass = ACharacterPlayer::StaticClass();
	PlayerStateClass = AOutBreakPlayerState::StaticClass();
}

void AOBGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACharacterSpawnManager* SpawnManager = GetWorld()->SpawnActor<ACharacterSpawnManager>(ACharacterSpawnManager::StaticClass(), SpawnParams);
}
