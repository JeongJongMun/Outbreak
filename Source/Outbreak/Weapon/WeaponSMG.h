#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Outbreak/Util/Define.h"
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