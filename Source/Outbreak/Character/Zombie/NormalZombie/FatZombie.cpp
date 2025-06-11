// Fill out your copyright notice in the Description page of Project Settings.


#include "FatZombie.h"

AFatZombie::AFatZombie()
{
	ZombieSubType = EZombieSubType::Fat;
}

void AFatZombie::BeginPlay()
{
	SetMesh(ECharacterBodyType::Fat);
	SetPhysicalAsset(ECharacterType::Zombie, ECharacterBodyType::Fat);
	
	Super::BeginPlay();
}