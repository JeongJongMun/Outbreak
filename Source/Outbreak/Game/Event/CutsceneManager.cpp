// Fill out your copyright notice in the Description page of Project Settings.


#include "CutsceneManager.h"
#include "Outbreak/UI/OB_HUD.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Outbreak/Game/OutBreakGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"

void UCutsceneManager::Init(UWorld* InWorld)
{
	WorldRef = InWorld;
}

void UCutsceneManager::PlayCutscene(ULevelSequence* Sequence)
{
	if (!WorldRef || !Sequence) return;
	
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;

	ALevelSequenceActor* OutActor = nullptr;
	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(WorldRef, Sequence, Settings, OutActor);
	if (Player)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldRef, 0))
		{
			if (AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD()))
			{
				HUD->SetCutsceneMode(true);
			}
		}
		APlayerController* PC = UGameplayStatics::GetPlayerController(WorldRef, 0);
		if (PC && PC->IsLocalController())
		{
			PC->DisableInput(PC);
			if (APawn* Pawn = PC->GetPawn())
			{
				if (ACharacter* Character = Cast<ACharacter>(Pawn))
				{
					Character->GetCharacterMovement()->DisableMovement();
					if (ACharacterPlayer* CP = Cast<ACharacterPlayer>(Character))
					{
						CP->bIsCutscenePlaying = true;
					}
				}
			}
		}
		Player->OnFinished.AddDynamic(this, &UCutsceneManager::OnCutSceneFinished);
		Player->Play();
	}
}

void UCutsceneManager::OnCutSceneFinished()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldRef, 0);
	if (PC && PC->IsLocalController())
	{
		PC->EnableInput(PC);
		if (APawn* Pawn = PC->GetPawn())
		{
			if (ACharacter* Character = Cast<ACharacter>(Pawn))
			{
				Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				if (ACharacterPlayer* CP = Cast<ACharacterPlayer>(Character))
				{
					CP->bIsCutscenePlaying = false;
				}
			}
		}
	}
	if (WorldRef && WorldRef->GetAuthGameMode() != nullptr)
	{
		if (AOutBreakGameState* GS = WorldRef->GetGameState<AOutBreakGameState>())
		{
			GS->SpawnerSetup();
		}
	}
	if (APlayerController* PC2 = UGameplayStatics::GetPlayerController(WorldRef, 0))
	{
		if (AOB_HUD* HUD = Cast<AOB_HUD>(PC2->GetHUD()))
		{
			HUD->SetCutsceneMode(false);
		}
	}
}
