// Fill out your copyright notice in the Description page of Project Settings.


#include "OutbreakGamemodeBase.h"

void AOutbreakGamemodeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		GEngine -> AddOnScreenDebugMessage(-1, 5, FColor::Red, "GameModeBase");
	}
}
