// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

UCLASS()
class OUTBREAK_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void SetObjectiveText(const FString& Text);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Objective;
};
