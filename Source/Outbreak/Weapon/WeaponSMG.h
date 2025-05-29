#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponSMG.generated.h"

UCLASS()
class OUTBREAK_API AWeaponSMG : public AWeaponBase
{
	GENERATED_BODY()

public:
	AWeaponSMG();

	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Reload() override;

protected:
	void MakeShot();
	void FinishReload();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceMaxDistance = 5000.0f;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Recoil")
	float BulletSpread = 7.0f; 

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Sound")
	USoundBase* SMGShotSound;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	float FireFrequency = 0.05f;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Ammo")
	int32 MagazineCapacity = 30;

	UPROPERTY(EditAnywhere, Category="Weapon|Ammo")
	int32 TotalAmmo = 120;

	UPROPERTY(VisibleAnywhere, Category="Weapon|Ammo")
	int32 CurrentAmmo;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Ammo")
	float ReloadDuration = 1.5f;
	
	bool bIsReloading = false;

	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle ReloadTimerHandle;
};