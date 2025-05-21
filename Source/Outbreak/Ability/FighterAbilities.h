// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbility.h"
#include "FighterAbilities.generated.h"

// 2-2 힘이 넘쳐!
UCLASS()
class OUTBREAK_API UAbilityFullOfStrength : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 3-1 카운터 공격
UCLASS()
class OUTBREAK_API UAbilityCounterAttack : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 3-2 강인함
UCLASS()
class OUTBREAK_API UAbilityStrength : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 4-1 분노!
UCLASS()
class OUTBREAK_API UAbilityRage : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 4-2 강철 몸
UCLASS()
class OUTBREAK_API UAbilityPowerBody : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 5-1 격투 본능
UCLASS()
class OUTBREAK_API UAbilityFightInstinct : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 5-2 최후의 발악
UCLASS()
class OUTBREAK_API UAbilityLastDance : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};