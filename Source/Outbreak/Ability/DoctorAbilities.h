// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbility.h"
#include "DoctorAbilities.generated.h"



// 2-2 회복이 우선
UCLASS()
class OUTBREAK_API UAbilityHealFirst : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 3-1 응급 처치
UCLASS()
class OUTBREAK_API UAbilityFirstAid : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 3-2 처방 전문가
UCLASS()
class OUTBREAK_API UAbilityPrescribingSpecialist : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 4-1 약물 강화
UCLASS()
class OUTBREAK_API UAbilityDrugEnhancement : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 4-2 부활 전문가
UCLASS()
class OUTBREAK_API UAbilityReviveSpecialist : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 5-1 생명의 손길
UCLASS()
class OUTBREAK_API UAbilityTouchOfLife : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 5-2 불사신
UCLASS()
class OUTBREAK_API UAbilityImmortal : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};