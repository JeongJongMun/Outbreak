// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Outbreak/Util/Define.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
class OUTBREAK_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
	
// --------------------
// Functions
// --------------------
public:
	ACharacterBase();
	virtual void TakeHitDamage(const FHitResult& HitResult, int32 BaseDamage);

protected:
	virtual void SetPhysicalAsset(ECharacterType CharacterType, ECharacterBodyType BodyType);
	virtual bool IsDead() const;
	virtual void Die();
	virtual float GetDamageMultiplier(EPhysicalSurface SurfaceType);
	virtual void ApplyDamage(int32 DamageAmount);
	virtual void ApplyHitEffects(EPhysicalSurface SurfaceType, int32 DamageAmount);
	
// --------------------
// Variables
// --------------------
public:
	float AttackDamageMultiplier = 1.0f;
	
protected:
	int32 CurrentHealth;
	int32 CurrentExtraHealth;

	// TODO : Hit Damage Multiplier Data Table
	float HeadDamageMultiplier = 3.0f;
	float BodyDamageMultiplier = 1.0f;
	float LimbsDamageMultiplier = 0.7f;
};
