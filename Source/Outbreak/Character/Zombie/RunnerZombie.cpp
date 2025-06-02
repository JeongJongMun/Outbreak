// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerZombie.h"

ARunnerZombie::ARunnerZombie()
{
	
}

void ARunnerZombie::BeginPlay()
{
	SetMesh();
	
	Super::BeginPlay();
	
	MontageSectionNameMap.Remove(EZombieStateType::Chase);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseRunSectionName);
}

void ARunnerZombie::SetMesh(EZombieMeshType MeshType)
{
	Super::SetMesh(MeshType);
}
