// WeaponAR.cpp
#include "WeaponAR.h"
#include "UObject/ConstructorHelpers.h"
#include "ARAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/UI/OB_HUD.h"

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
    WeaponMesh->SetHiddenInGame(true);
    ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Game/Sounds/AR_Single.AR_Single"));
    if (tempSound.Succeeded()) {
        WeaponData.ShotSound = tempSound.Object; 
    }
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_WeaponData(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponDataTable.WeaponDataTable'"));
    if (DT_WeaponData.Succeeded())
    {
        WeaponDataTable = DT_WeaponData.Object;
    }
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireEffect(TEXT("/Game/MuzzleFlash/MuzzleFlash/Niagara/NS_MuzzleFlash.NS_MuzzleFlash"));
    if (FireEffect.Succeeded())
    {
        NiagaraMuzzleFlash = FireEffect.Object;
    }
}
void AWeaponAR::Reload()
{
    if (bIsReloading || WeaponData.CurrentAmmo == WeaponData.MagazineCapacity || WeaponData.TotalAmmo <= 0)
        return;

    bIsReloading = true;
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
    NotifyAmmoUpdate();
}

void AWeaponAR::FinishReload()
{
    int32 Needed = WeaponData.MagazineCapacity - WeaponData.CurrentAmmo;
    int32 ToReload = FMath::Min(Needed, WeaponData.TotalAmmo);

    WeaponData.CurrentAmmo += ToReload;
    WeaponData.TotalAmmo -= ToReload;
    bIsReloading = false;

    UE_LOG(LogTemp, Log, TEXT("Reloaded: %d / %d"), WeaponData.CurrentAmmo, WeaponData.TotalAmmo);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("%d / %d"), WeaponData.CurrentAmmo, WeaponData.TotalAmmo));
    }
    NotifyAmmoUpdate();
}

void AWeaponAR::StartFire()
{
    if (bIsReloading)
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

    if (bIsReloading)
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
    FVector MuzzleLoc = WeaponMesh->GetSocketLocation(MuzzleSocketName);

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
    Params.bReturnPhysicalMaterial=true;
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, ViewLocation, TraceEnd, ECC_Visibility, Params);


    // 5) 총구 소켓 위치



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
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor -> IsA(ACharacterZombie::StaticClass()))
        {
            UGameplayStatics::ApplyPointDamage(
                HitActor,
                50.0f,
                GetActorForwardVector(),
                Hit,
                PC,
                this,
                UDamageType::StaticClass());
        }
    }
    PlayMuzzleEffect();
    NotifyAmmoUpdate();
}


void AWeaponAR::InitializeWeaponData(FWeaponData* InData)
{
    WeaponData = *InData;
    WeaponData.CurrentAmmo = WeaponData.MagazineCapacity;
}

bool AWeaponAR::IsReloading()
{
    return bIsReloading;
}

void AWeaponAR::NotifyAmmoUpdate()
{
    ACharacterPlayer* OwnerCharacter = Cast<ACharacterPlayer>(GetOwner());
    if (!OwnerCharacter) return;

    APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
    if (!PC) return;

    AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD());
    if (HUD)
    {
        HUD->DisplayAmmo(WeaponData.CurrentAmmo, WeaponData.TotalAmmo);
    }
}

void AWeaponAR::BeginPlay()
{
    Super::BeginPlay();
    if (!HasAuthority())
    {
        return;
    }

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

void AWeaponAR::PlayMuzzleEffect()
{
    FRotator MuzzleRot = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
        NiagaraMuzzleFlash, 
        WeaponMesh,                     
        TEXT("Muzzle_AR"),
        FVector::ZeroVector,
        FRotator(MuzzleRot.Pitch, MuzzleRot.Yaw + 90.0f, MuzzleRot.Roll),
        EAttachLocation::SnapToTarget,
        true,   
        true   
    );

}


