// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkerZombie.h"

void AWalkerZombie::BeginPlay()
{
	Super::BeginPlay();

	MontageSectionNameMap.Remove(EZombieStateType::Chase);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseWalkSectionName);
}
