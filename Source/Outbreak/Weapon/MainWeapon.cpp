// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Outbreak/Util/CameraShake.h"

AMainWeapon::AMainWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireEffect(
	TEXT("/Game/MuzzleFlash/MuzzleFlash/Niagara/NS_MuzzleFlash.NS_MuzzleFlash")
);
	if (FireEffect.Succeeded())
	{
		NiagaraMuzzleFlash = FireEffect.Object;
	}
	
	
}




void AMainWeapon::ApplyCameraShake()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->ClientStartCameraShake(UCameraShake::StaticClass());
	}
}
