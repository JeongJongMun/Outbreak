// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Outbreak/Character/CharacterBase.h"
#include "BaseAbilityObject.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API UBaseAbilityObject : public UObject, public IBaseAbility
{
	GENERATED_BODY()

public:
	virtual EAbilityActivationType GetActivationType() const override { return ActivationType; }
	virtual EAbilityType GetAbilityType() const override { return AbilityType; }
	virtual FString GetAbilityName() const override { return AbilityName; }
	
	virtual float GetCooldown() const override { return 0.0f; }
	virtual float GetRemainingCooldown() const override { return 0.0f;}
	
	virtual float GetDuration() const override { return 0.0f; }
	virtual float GetRemainingDuration() const override { return 0.0f; }
	
	virtual bool IsAbilityActive() const override { return false;}
	virtual bool CanUseAbility() const override { return false; }
	virtual bool UseAbility() override { return false;}

	void SetOwnerCharacter(ACharacterBase* InOwner) { Owner = InOwner;}
	TObjectPtr<ACharacterBase> GetOwner() const { return Owner; }

protected:
	virtual void ActivateAbility();
	virtual void DeactivateAbility();

protected:
	EAbilityActivationType ActivationType = EAbilityActivationType::None;
	EAbilityType AbilityType = EAbilityType::None;
	FString AbilityName = TEXT("Unnamed Ability");
	TObjectPtr<ACharacterBase> Owner;
};
