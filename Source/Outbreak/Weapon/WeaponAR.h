// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWeapon.h"
#include "Outbreak/Util/Define.h"
#include "WeaponAR.generated.h"

UCLASS()
class OUTBREAK_API AWeaponAR : public AMainWeapon
{
	GENERATED_BODY()
public:
	AWeaponAR();
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Reload() override;
	virtual void InitializeWeaponData(FWeaponData* InData) override;
	bool IsReloading();
protected:
	virtual void BeginPlay() override;
	void MakeShot();
	void FinishReload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FWeaponData WeaponData;
	
	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle ReloadTimerHandle;
};
