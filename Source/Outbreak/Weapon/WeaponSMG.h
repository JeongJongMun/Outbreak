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
	bool IsReloading();
protected:
	virtual void BeginPlay() override;
	void MakeShot();
	void FinishReload();
	virtual void NotifyAmmoUpdate() override;

	UFUNCTION(Server, Reliable,WithValidation)
	void ServerMakeShot();

	UFUNCTION(Client, Unreliable)
	void ClientShotRay(const FVector& TraceEnd, bool bHit, const FVector& ImpactPoint);

	UFUNCTION(NetMulticast,Unreliable)
	void MultiCastShot(AController* ShooterController);

	void PlayLocalEffects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FWeaponData WeaponData;
	
	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle ReloadTimerHandle;
	
};