// WeaponBase.cpp
#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Outbreak/Util/CameraShake.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	
}

void AWeaponBase::StartFire()
{
	
}

void AWeaponBase::StopFire()
{
	
}

void AWeaponBase::Reload()
{
	
}
void AWeaponBase::ApplyCameraShake()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->ClientStartCameraShake(UCameraShake::StaticClass());
	}
}