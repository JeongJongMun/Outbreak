// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponAR.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AWeaponAR : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponAR();
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:
	// 실제 한 발을 발사하는 함수
	void MakeShot();

	// 히트스캔 사거리 (in cm)
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceMaxDistance = 5000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Recoil")
	float BulletSpread = 3.0f; 
	// 발사 간격을 위한 타이머 핸들
	FTimerHandle TimerHandle_TimeBetweenShots;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Sound")
	USoundBase* ARShotSound;
};
