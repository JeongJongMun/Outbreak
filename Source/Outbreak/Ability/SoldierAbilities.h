// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbility.h"
#include "SoldierAbilities.generated.h"

// 현재는 BasicCharacter에서 포인터를 가져오지만 추후에 군인만 가지는 어빌리티는 SoldierCharacter* 로 변경해야할것 으로 보임
// 추가로 피버 모드 능력은 공용 어빌리티 이므로 추후에 따로 빼서 공용 어빌리티 전용 스크립트를 만들어 관리할 것
// 2-1 피버 타임
UCLASS()
class OUTBREAK_API UAbilityFeverTime : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
	public:
		virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 2-2 사격 개시!
UCLASS()
class OUTBREAK_API UAbilityStartShooting : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 3-1 명사수
UCLASS()
class OUTBREAK_API UAbilitySharpShooter : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 3-2 Fire in the Hole
UCLASS()
class OUTBREAK_API UAbilityFireInTheHole : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 4-1 강약약강
UCLASS()
class OUTBREAK_API UAbilityStrongAndWeak : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 4-2 Attention!
UCLASS()
class OUTBREAK_API UAbilityAttention : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 5-1 광란의 사수
UCLASS()
class OUTBREAK_API UAbilityMadShooter : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};

// 5-2 특급 전사
UCLASS()
class OUTBREAK_API UAbilityExpressWarrior : public UObject, public ICharacterAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(TObjectPtr<ACharacterBase> Character) override;
};