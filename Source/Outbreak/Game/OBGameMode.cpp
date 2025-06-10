// Fill out your copyright notice in the Description page of Project Settings.

#include "OBGameMode.h"

#include "OutBreakGameState.h"
#include "OutBreakPlayerState.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Character/Zombie/CharacterSpawnManager.h"
#include "Outbreak/UI/OB_HUD.h"

AOBGameMode::AOBGameMode()
{
    DefaultPawnClass = ACharacterPlayer::StaticClass();
	PlayerStateClass = AOutBreakPlayerState::StaticClass();
	GameStateClass = AOutBreakGameState::StaticClass();
}

void AOBGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}
