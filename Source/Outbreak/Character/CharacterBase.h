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
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION()
	virtual void OnRep_CurrentHealth();

protected:
	virtual void BeginPlay() override;
	virtual void InitCharacterData();
	virtual void SetupCollision();
	virtual void SetupMovement();
	virtual void SetPhysicalAsset(ECharacterType InCharacterType, ECharacterBodyType InBodyType);
	virtual bool IsDead() const;
	virtual void Die();
	virtual float GetDamageMultiplier(EPhysicalSurface SurfaceType);
	virtual void ApplyDamage(int32 DamageAmount);
	virtual void ApplyHitEffects(const int32 DamageAmount, const EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default);

	
// --------------------
// Variables
// --------------------
public:
	float AttackDamageMultiplier = 1.0f;
	
protected:
	ECharacterType CharacterType = ECharacterType::None;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	int32 CurrentHealth = 100;

	int32 CurrentExtraHealth = 0;

	// TODO : Hit Damage Multiplier Data Table
	float HeadDamageMultiplier = 3.0f;
	float BodyDamageMultiplier = 1.0f;
	float LimbsDamageMultiplier = 0.7f;

	float CurrentAnimationSectionLength = 0.0f;
};
