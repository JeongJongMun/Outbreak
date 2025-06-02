// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkerZombie.h"
#include "Outbreak/Util/MeshLoadHelper.h"

AWalkerZombie::AWalkerZombie()
{

}

void AWalkerZombie::BeginPlay()
{
	SetMesh();
	
	Super::BeginPlay();
	
	MontageSectionNameMap.Remove(EZombieStateType::Chase);
	MontageSectionNameMap.Add(EZombieStateType::Chase, ChaseWalkSectionName);
}

void AWalkerZombie::SetMesh(EZombieMeshType MeshType)
{
	Super::SetMesh(MeshType);
}
