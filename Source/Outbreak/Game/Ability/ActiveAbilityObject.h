// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbilityObject.h"
#include "ActiveAbilityObject.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API UActiveAbilityObject : public UBaseAbilityObject
{
	GENERATED_BODY()

public:
	UActiveAbilityObject()
	{
		ActivationType = EAbilityActivationType::Active;
	}

	virtual float GetCooldown() const override { return Cooldown; }
	virtual float GetRemainingCooldown() const override { return RemainingCooldown; }
	
	virtual float GetDuration() const override { return Duration; }
	virtual float GetRemainingDuration() const override { return RemainingDuration; }
	
	virtual bool IsAbilityActive() const override { return bIsActive; }
	virtual bool CanUseAbility() const override { return GetRemainingCooldown() <= 0.f && !bIsActive; }
	virtual bool UseAbility() override;

	void TickCooldown(float DeltaTime);
	void TickDuration(float DeltaTime);

protected:
	float Cooldown = 0.0f;
	float Duration = 0.0f;
	float RemainingCooldown = 0.0f;
	float RemainingDuration = 0.0f;
	bool bIsActive = false;
};
