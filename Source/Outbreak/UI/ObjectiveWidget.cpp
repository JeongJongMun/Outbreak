// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWidget.h"
#include "Components/TextBlock.h"


void UObjectiveWidget::SetObjectiveText(const FString& Text)
{
	if (Text_Objective)
	{
		Text_Objective->SetText(FText::FromString(Text));
	}
}

