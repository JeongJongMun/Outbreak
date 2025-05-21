// WeaponAR.cpp
#include "WeaponAR.h"
#include "UObject/ConstructorHelpers.h"
#include "ARAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponAR, All, All);
AWeaponAR::AWeaponAR()
{
    AmmoClass = AARAmmo::StaticClass();
    
    MuzzleSocketName = TEXT("Muzzle_AR");
    
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshObj(
        TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/AR2/SM_weapon_AR2.SM_weapon_AR2"));
    if (WeaponMeshObj.Succeeded())
    {
        WeaponMesh->SetSkeletalMesh(WeaponMeshObj.Object);
    }
    ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Game/Sounds/AR_Single.AR_Single"));
    if (tempSound.Succeeded()) {
        ARShotSound = tempSound.Object; 
    }

    
}


void AWeaponAR::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(
        TimerHandle_TimeBetweenShots,
        this,
        &AWeaponAR::MakeShot,
        FireFrequency,
        true
    );
}

void AWeaponAR::StopFire()
{

    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AWeaponAR::MakeShot()
{
    ApplyCameraShake();
    UGameplayStatics::PlaySound2D(GetWorld(), ARShotSound);


    AActor* MyOwner = GetOwner();
    if (!MyOwner)
    {

        return;
    }


    APlayerController* PC = Cast<APlayerController>(MyOwner->GetInstigatorController());
    if (!PC)
    {

        return;
    }

    // 1) 카메라 뷰포인트 가져오기
    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    // 2) 분산(스프레드) 적용
    const float HalfRad = FMath::DegreesToRadians(BulletSpread * 0.5f);
    FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    
    // 3) TraceEnd 계산
    FVector TraceEnd = ViewLocation + ShotDirection * TraceMaxDistance;


    // 4) 히트 검사
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(MyOwner);
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, ViewLocation, TraceEnd, ECC_Visibility, Params);


    // 5) 총구 소켓 위치
    FVector MuzzleLoc = WeaponMesh->GetSocketLocation(MuzzleSocketName);


    FVector EndPoint = bHit ? Hit.ImpactPoint : TraceEnd;
    
    // 7) 디버그 드로잉
    DrawDebugLine(
        GetWorld(),
        MuzzleLoc,
        EndPoint,
        FColor::Red,
        false,
        2.0f,
        SDPG_World,
        2.5f
    );

    if (bHit)
    {
        DrawDebugSphere(
            GetWorld(),
            Hit.ImpactPoint,
            6.0f,
            12,
            FColor::Yellow,
            false,
            2.0f
        );
    }


}

