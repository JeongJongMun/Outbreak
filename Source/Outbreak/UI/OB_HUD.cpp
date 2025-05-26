// Fill out your copyright notice in the Description page of Project Settings.


#include "OB_HUD.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.h"
void AOB_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (UClass* WidgetClass = LoadClass<UObjectiveWidget>(nullptr, TEXT("/Game/UI/WBP_Objective.WBP_Objective_C")))
	{
		ObjectiveWidget = CreateWidget<UObjectiveWidget>(GetWorld(), WidgetClass);
		if (ObjectiveWidget)
		{
			ObjectiveWidget->AddToViewport();
			ObjectiveWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AOB_HUD::DisplayObjectiveMessage(const FString& Message)
{
	if (ObjectiveWidget)
	{
		ObjectiveWidget->SetObjectiveText(Message);
		ObjectiveWidget->SetVisibility(ESlateVisibility::Visible);
	}
}