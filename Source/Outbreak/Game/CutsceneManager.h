// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelSequence.h"
#include "CoreMinimal.h"
#include "CutsceneManager.generated.h"


UCLASS()
class OUTBREAK_API UCutsceneManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UWorld* InWorld);

	void PlayCutscene(ULevelSequence* Sequence, const FString& InObjectiveMessage);

	bool bHasPlayedCutscene = false;

private:
	UPROPERTY()
	UWorld* WorldRef;

	FString PendingObjectiveMessage;

	UFUNCTION()
	void OnShowObjectiveMessage();

	UFUNCTION()
	void OnCutSceneFinished();
};
