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

    WeaponData.CurrentAmmo = WeaponData.MagazineCapacity;
    
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshObj(
        TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/AR2/SM_weapon_AR2.SM_weapon_AR2"));
    if (WeaponMeshObj.Succeeded())
    {
        WeaponMesh->SetSkeletalMesh(WeaponMeshObj.Object);
    }
    ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Game/Sounds/AR_Single.AR_Single"));
    if (tempSound.Succeeded()) {
        WeaponData.ShotSound = tempSound.Object; 
    }

    
}
void AWeaponAR::Reload()
{
    if (WeaponData.bIsReloading || WeaponData.CurrentAmmo == WeaponData.MagazineCapacity || WeaponData.TotalAmmo <= 0)
        return;

    WeaponData.bIsReloading = true;
    StopFire();

    // 시작 시 리로드 애니메이션 재생 (추가 가능)

    // reload 완료 시점까지 대기
    GetWorldTimerManager().SetTimer(
        ReloadTimerHandle,
        this,
        &AWeaponAR::FinishReload,
        WeaponData.ReloadDuration,
        false
    );
}

void AWeaponAR::FinishReload()
{
    int32 Needed = WeaponData.MagazineCapacity - WeaponData.CurrentAmmo;
    int32 ToReload = FMath::Min(Needed, WeaponData.TotalAmmo);

    WeaponData.CurrentAmmo += ToReload;
    WeaponData.TotalAmmo -= ToReload;
    WeaponData.bIsReloading = false;

    UE_LOG(LogTemp, Log, TEXT("Reloaded: %d / %d"), WeaponData.CurrentAmmo, WeaponData.TotalAmmo);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("%d / %d"), WeaponData.CurrentAmmo, WeaponData.TotalAmmo));
    }
}

void AWeaponAR::StartFire()
{
    if (WeaponData.bIsReloading)
        return;

    if (WeaponData.CurrentAmmo <= 0)
    {
        Reload();
        return;
    }
    
    MakeShot();
    GetWorldTimerManager().SetTimer(
        TimerHandle_TimeBetweenShots,
        this,
        &AWeaponAR::MakeShot,
        WeaponData.FireFrequency,
        true
    );
}

void AWeaponAR::StopFire()
{

    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AWeaponAR::MakeShot()
{

    if (WeaponData.bIsReloading)
        return;

    if (WeaponData.CurrentAmmo <= 0)
    {
        StopFire();
        Reload();
        return;
    }

    WeaponData.CurrentAmmo--;

    ApplyCameraShake();
    UGameplayStatics::PlaySound2D(GetWorld(), WeaponData.ShotSound);


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
    const float HalfRad = FMath::DegreesToRadians(WeaponData.BulletSpread * 0.5f);
    FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    
    // 3) TraceEnd 계산
    FVector TraceEnd = ViewLocation + ShotDirection * WeaponData.TraceMaxDistance;


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

void AWeaponAR::InitializeWeaponData(FWeaponData* InData)
{
    WeaponData = *InData;
}

bool AWeaponAR::IsReloading()
{
    return WeaponData.bIsReloading;
}

void AWeaponAR::BeginPlay()
{
    Super::BeginPlay();
    
    if (WeaponDataTable)
    {
        static const FString ContextString(TEXT("WeaponAR DataTable Initialization"));
        
        FWeaponData* FoundRow = WeaponDataTable->FindRow<FWeaponData>(FName(TEXT("WeaponAR")), ContextString, /*bWarnIfNotFound=*/ true);
        if (FoundRow)
            InitializeWeaponData(FoundRow);
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[WeaponAR] WeaponDataTable에서 'WeaponAR' 행을 찾지 못했습니다."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[WeaponAR] WeaponDataTable이 에디터에 연결되지 않았습니다."));
    }
}


