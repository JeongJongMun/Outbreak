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
	void ApplyCameraShake();
	FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	
protected:

	void MakeShot();
	void FinishReload();

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AAmmoBase> AmmoClass; // 사용할 탄약 클래스
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName MuzzleSocketName = TEXT("Muzzle"); // 발사될 총구 소켓 이름
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh; // 무기 메시

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	UDataTable* WeaponDataTable;

};
