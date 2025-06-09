// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkerZombie.h"
#include "Outbreak/Util/MeshLoadHelper.h"

AWalkerZombie::AWalkerZombie()
{
	ZombieSubType = EZombieSubType::Walker;
}

void AWalkerZombie::BeginPlay()
{
	SetMesh();
	SetPhysicalAsset(ECharacterType::Zombie, ECharacterBodyType::Normal);
	
	Super::BeginPlay();
	
	MontageSectionNameMap.Remove(EZombieStateType::Chase);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseWalkSectionName);
}

void AWalkerZombie::SetMesh(ECharacterBodyType MeshType)
{
	Super::SetMesh(MeshType);
}
