// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseAbility.generated.h"

UENUM()
enum class EAbilityType : uint8
{
	None            UMETA(DisplayName = "None"),
	DamageEnhance	UMETA(DisplayName = "DamageEnhance"),
};

UENUM()
enum class EAbilityActivationType : uint8
{
	None	UMETA(DisplayName = "None"),
	Passive	UMETA(DisplayName = "Passive"),
	Active	UMETA(DisplayName = "Active"),
};

UINTERFACE(MinimalAPI)
class UBaseAbility : public UInterface
{
	GENERATED_BODY()
};

class OUTBREAK_API IBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual EAbilityActivationType GetActivationType() const = 0;
	virtual EAbilityType GetAbilityType() const = 0;
	virtual FString GetAbilityName() const = 0;
	
	virtual float GetCooldown() const = 0;
	virtual float GetRemainingCooldown() const = 0;
	
	virtual float GetDuration() const = 0;
	virtual float GetRemainingDuration() const = 0;
	
	virtual bool IsAbilityActive() const = 0;
	virtual bool CanUseAbility() const = 0;
	virtual bool UseAbility() = 0;
};
