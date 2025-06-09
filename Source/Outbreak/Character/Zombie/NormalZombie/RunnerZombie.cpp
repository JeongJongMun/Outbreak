// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerZombie.h"

ARunnerZombie::ARunnerZombie()
{
	ZombieSubType = EZombieSubType::Runner;
}

void ARunnerZombie::BeginPlay()
{
	SetMesh();
	SetPhysicalAsset(ECharacterType::Zombie, ECharacterBodyType::Normal);
	
	Super::BeginPlay();
	
	MontageSectionNameMap.Remove(EZombieStateType::Chase);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseRunSectionName);
}

void ARunnerZombie::SetMesh(ECharacterBodyType MeshType)
{
	Super::SetMesh(MeshType);
}
