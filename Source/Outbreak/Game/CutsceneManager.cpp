// Fill out your copyright notice in the Description page of Project Settings.


#include "CutsceneManager.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

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
		Player->Play();
	}
}
