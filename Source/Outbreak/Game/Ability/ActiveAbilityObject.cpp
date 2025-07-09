// Fill out your copyright notice in the Description page of Project Settings.

#include "ActiveAbilityObject.h"

bool UActiveAbilityObject::UseAbility()
{
	if (!CanUseAbility())
		return false;

	ActivateAbility();
	RemainingCooldown = Cooldown;
	RemainingDuration = Duration;
	bIsActive = true;
	
	return true;
}

void UActiveAbilityObject::TickCooldown(float DeltaTime)
{
	if (RemainingCooldown > 0.0f)
	{
		RemainingCooldown = FMath::Max(RemainingCooldown - DeltaTime, 0.0f);
	}
}

void UActiveAbilityObject::TickDuration(float DeltaTime)
{
	if (bIsActive)
	{
		RemainingDuration = FMath::Max(RemainingDuration - DeltaTime, 0.0f);
		if (RemainingDuration <= 0.0f)
		{
			DeactivateAbility();
			bIsActive = false;
		}
	}
}
