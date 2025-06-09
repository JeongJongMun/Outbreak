// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PassiveAbilityObject.h"
#include "DamageEnhanceAbility.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API UDamageEnhanceAbility : public UPassiveAbilityObject
{
	GENERATED_BODY()

public:
	UDamageEnhanceAbility()
	{
		AbilityType = EAbilityType::DamageEnhance;
		AbilityName = TEXT("DamageEnhance");
	}
	
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

private:
	float DamageMultiplier = 2.0f;

};
