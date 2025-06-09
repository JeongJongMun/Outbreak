// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageEnhanceAbility.h"

void UDamageEnhanceAbility::ActivateAbility()
{
	Super::ActivateAbility();

	TObjectPtr<ACharacterBase> OwnerCharacter = GetOwnerCharacter();
	OwnerCharacter->AttackDamageMultiplier *= DamageMultiplier;
	// TODO : Implement logic to apply the damage enhancement to the owner character (both zombie and player)
}

void UDamageEnhanceAbility::DeactivateAbility()
{
	Super::DeactivateAbility();
	
	TObjectPtr<ACharacterBase> OwnerCharacter = GetOwnerCharacter();
	OwnerCharacter->AttackDamageMultiplier /= DamageMultiplier;
	// TODO : Implement logic to revert the damage enhancement from the owner character (both zombie and player)
}