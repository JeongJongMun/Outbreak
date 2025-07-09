// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelSequence.h"
#include "CoreMinimal.h"
#include "CutsceneController.generated.h"


UCLASS()
class OUTBREAK_API UCutsceneController : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UWorld* InWorld);

	void PlayCutscene(ULevelSequence* Sequence);

	bool bHasPlayedCutscene = false;

private:
	UPROPERTY()
	UWorld* WorldRef;

	UFUNCTION()
	void OnCutSceneFinished();
};
