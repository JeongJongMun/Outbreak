// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.h"
#include "MainWeapon.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AMainWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AMainWeapon();
	
	void ApplyCameraShake();


protected:

	void MakeShot();
	void FinishReload();

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AAmmoBase> AmmoClass; // 사용할 탄약 클래스
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName MuzzleSocketName = TEXT("Muzzle"); // 발사될 총구 소켓 이름
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	UDataTable* WeaponDataTable;

};
