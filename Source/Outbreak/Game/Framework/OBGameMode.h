// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OBGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AOBGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AOBGameMode();
	
protected:
	virtual void BeginPlay() override;
	
};
