#include "WeaponSMG.h"
#include "SMGAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AWeaponSMG::AWeaponSMG()
{
    AmmoClass = ASMGAmmo::StaticClass();
    MuzzleSocketName = TEXT("Muzzle_SMG");

    CurrentAmmo = MagazineCapacity;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshObj(
        TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/SMG02/SK_weapon_SMG_02.SK_weapon_SMG_02"));
    if (WeaponMeshObj.Succeeded())
    {
        WeaponMesh->SetSkeletalMesh(WeaponMeshObj.Object);
    }
    
    ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Game/Sounds/AR_Single.AR_Single"));
    if (tempSound.Succeeded()) {
        SMGShotSound = tempSound.Object; 
    }
}

void AWeaponSMG::StartFire()
{
    if (bIsReloading)
        return;

    if (CurrentAmmo <= 0)
    {
        Reload();
        return;
    }

    MakeShot();
    GetWorldTimerManager().SetTimer(
        TimerHandle_TimeBetweenShots,
        this,
        &AWeaponSMG::MakeShot,
        FireFrequency,
        true
    );
}

void AWeaponSMG::StopFire()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AWeaponSMG::Reload()
{
    if (bIsReloading || CurrentAmmo == MagazineCapacity || TotalAmmo <= 0)
        return;

    bIsReloading = true;
    StopFire();

    // 시작 시 리로드 애니메이션 재생 (추가 가능)

    // reload 완료 시점까지 대기
    GetWorldTimerManager().SetTimer(
        ReloadTimerHandle,
        this,
        &AWeaponSMG::FinishReload,
        ReloadDuration,
        false
    );
}

void AWeaponSMG::FinishReload()
{
    int32 Needed = MagazineCapacity - CurrentAmmo;
    int32 ToReload = FMath::Min(Needed, TotalAmmo);

    CurrentAmmo += ToReload;
    TotalAmmo -= ToReload;
    bIsReloading = false;

    UE_LOG(LogTemp, Log, TEXT("Reloaded: %d / %d"), CurrentAmmo, TotalAmmo);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("%d / %d"), CurrentAmmo, TotalAmmo));
    }
}

void AWeaponSMG::MakeShot()
{
    if (bIsReloading)
        return;

    if (CurrentAmmo <= 0)
    {
        StopFire();
        Reload();
        return;
    }

    CurrentAmmo--;

    ApplyCameraShake();
    UGameplayStatics::PlaySound2D(GetWorld(), SMGShotSound);

    AActor* MyOwner = GetOwner();
    if (!MyOwner) return;

    APlayerController* PC = Cast<APlayerController>(MyOwner->GetInstigatorController());
    if (!PC) return;

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const float HalfRad = FMath::DegreesToRadians(BulletSpread * 0.5f);
    FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    FVector TraceEnd = ViewLocation + ShotDirection * TraceMaxDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(MyOwner);
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, ViewLocation, TraceEnd, ECC_Visibility, Params);

    FVector MuzzleLoc = WeaponMesh->GetSocketLocation(MuzzleSocketName);
    FVector EndPoint = bHit ? Hit.ImpactPoint : TraceEnd;

    DrawDebugLine(
        GetWorld(),
        MuzzleLoc,
        EndPoint,
        FColor::Cyan,
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
            FColor::Yellow
        );
    }
}

bool AWeaponSMG::IsReloading()
{
    return bIsReloading;
}

