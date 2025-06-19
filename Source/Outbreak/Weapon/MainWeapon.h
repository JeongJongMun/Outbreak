// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.h"
#include "NiagaraSystem.h"       
#include "NiagaraFunctionLibrary.h"     
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
	TSubclassOf<AAmmoBase> AmmoClass;
	FName MuzzleSocketName;
	TObjectPtr<UNiagaraSystem> NiagaraMuzzleFlash;
	
	UPROPERTY()
	TObjectPtr<UDataTable> WeaponDataTable;
};
