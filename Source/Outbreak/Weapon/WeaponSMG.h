// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponSMG.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AWeaponSMG : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponSMG();

	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:
	void MakeShot();


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceMaxDistance = 5000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Recoil")
	float BulletSpread = 7.0f; 

	FTimerHandle TimerHandle_TimeBetweenShots;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Sound")
	USoundBase* SMGShotSound;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	float FireFrequency = 0.05f; // 발사 간격
};
