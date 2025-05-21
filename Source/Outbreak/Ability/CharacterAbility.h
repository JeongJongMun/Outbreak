// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAbility.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterAbility : public UInterface
{
	GENERATED_BODY()
};

class OUTBREAK_API ICharacterAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(TObjectPtr<class ACharacterBase> Character) = 0;
};
