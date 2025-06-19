#pragma once

#include "CoreMinimal.h"
#include "MainWeapon.h"
#include "Outbreak/Util/Define.h"
#include "WeaponSMG.generated.h"

UCLASS()
class OUTBREAK_API AWeaponSMG : public AMainWeapon
{
	GENERATED_BODY()

public:
	AWeaponSMG();
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Reload() override;
	virtual void InitializeWeaponData(FWeaponData* InData) override;
	bool IsReloading() const { return bIsReloading; }
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void MakeShot();

	void FinishReload();
	virtual void NotifyAmmoUpdate() override;

	UFUNCTION(Server,Reliable)
	void ServerStartFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();
	
	UFUNCTION(NetMulticast,Unreliable)
	void MultiCastShot();
	
	void PlayMuzzleEffect();

protected:
	UPROPERTY(Replicated)
	FWeaponData WeaponData;
	
	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle ReloadTimerHandle;	
	
};