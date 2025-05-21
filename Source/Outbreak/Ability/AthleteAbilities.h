// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CharacterAbility.h"
#include "AthleteAbilities.generated.h"

// 2-2 나보다 빨라?
UCLASS()
class OUTBREAK_API UAbilityFastThanMe : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};

// 3-1 회피 본능
UCLASS()
class OUTBREAK_API UAbilityAvoidanceInstinct : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};

// 3-2 속도광
UCLASS()
class OUTBREAK_API UAbilitySpeedFreak : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};

// 4-1 지구력왕
UCLASS()
class OUTBREAK_API UAbilityEnduranceKing : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};

// 4-2 도주 전문가
UCLASS()
class OUTBREAK_API UAbilityEscapeSpecialist : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};

// 5-1 질주 본능
UCLASS()
class OUTBREAK_API UAbilitySprintInstinct : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};

// 5-2 마라토너
UCLASS()
class OUTBREAK_API UAbilityMarathoner : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase>) override;
};
