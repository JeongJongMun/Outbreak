// WeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Outbreak/Util/Define.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class AAmmoBase;

UCLASS(Abstract)
class OUTBREAK_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();
	virtual void StartFire();
	virtual void StopFire();
	virtual void Reload();
	virtual void InitializeWeaponData(FWeaponData* InData);
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh; // 무기 메시
	bool bIsReloading = false;
};
