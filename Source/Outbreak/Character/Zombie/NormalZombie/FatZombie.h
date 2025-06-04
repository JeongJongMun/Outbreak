// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalZombie.h"
#include "Outbreak/Util/Define.h"
#include "FatZombie.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AFatZombie : public ANormalZombie
{
	GENERATED_BODY()

public:
	AFatZombie();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetMesh(ECharacterBodyType MeshType = ECharacterBodyType::Fat) override;
};
