// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbilityObject.h"
#include "PassiveAbilityObject.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API UPassiveAbilityObject : public UBaseAbilityObject
{
	GENERATED_BODY()

public:
	UPassiveAbilityObject()
	{
		ActivationType = EAbilityActivationType::Passive;
	}

	virtual bool UseAbility() override final { ActivateAbility(); return true; }
	virtual bool CanUseAbility() const override final { return false; }
	virtual bool IsAbilityActive() const override { return true; }
};