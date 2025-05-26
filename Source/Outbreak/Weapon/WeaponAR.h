// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponAR.generated.h"

UCLASS()
class OUTBREAK_API AWeaponAR : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponAR();
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:

	void MakeShot();


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceMaxDistance = 5000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Recoil")
	float BulletSpread = 3.0f; 

	FTimerHandle TimerHandle_TimeBetweenShots;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Sound")
	USoundBase* ARShotSound;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	float FireFrequency = 0.1f; // 발사 간격
};
