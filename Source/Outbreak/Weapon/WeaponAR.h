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
	virtual void Reload() override;
	
protected:

	void MakeShot();
	void FinishReload();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceMaxDistance = 5000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Recoil")
	float BulletSpread = 3.0f; 

	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle ReloadTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Sound")
	USoundBase* ARShotSound;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	float FireFrequency = 0.1f; // 발사 간격

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Ammo")
	int32 MagazineCapacity = 30;

	UPROPERTY(EditAnywhere, Category="Weapon|Ammo")
	int32 TotalAmmo = 120;

	UPROPERTY(VisibleAnywhere, Category="Weapon|Ammo")
	int32 CurrentAmmo;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Ammo")
	float ReloadDuration = 1.5f;
	
	bool bIsReloading = false;
};
