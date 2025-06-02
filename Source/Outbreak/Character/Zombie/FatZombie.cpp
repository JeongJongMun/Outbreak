// Fill out your copyright notice in the Description page of Project Settings.


#include "FatZombie.h"

AFatZombie::AFatZombie()
{
}

void AFatZombie::BeginPlay()
{
	SetMesh();
	
	Super::BeginPlay();

	MontageSectionNameMap.Remove(EZombieStateType::Chase);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseWalkSectionName);
}

void AFatZombie::SetMesh(EZombieMeshType MeshType)
{
	Super::SetMesh(MeshType);
}
