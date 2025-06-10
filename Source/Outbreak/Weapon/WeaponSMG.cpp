#include "WeaponSMG.h"
#include "SMGAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/UI/OB_HUD.h"

AWeaponSMG::AWeaponSMG()
{
    AmmoClass = ASMGAmmo::StaticClass();
    
    MuzzleSocketName = TEXT("Muzzle_SMG");

    WeaponData.CurrentAmmo = WeaponData.MagazineCapacity;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshObj(
        TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/SMG02/SK_weapon_SMG_02.SK_weapon_SMG_02"));
    if (WeaponMeshObj.Succeeded())
    {
        WeaponMesh->SetSkeletalMesh(WeaponMeshObj.Object);
    }
    
    ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Game/Sounds/AR_Single.AR_Single"));
    if (tempSound.Succeeded()) {
        WeaponData.ShotSound = tempSound.Object; 
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_WeaponData(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponDataTable.WeaponDataTable'"));
    if (DT_WeaponData.Succeeded())
    {
        WeaponDataTable = DT_WeaponData.Object;
    }
}
void AWeaponSMG::Reload()
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
        &AWeaponSMG::FinishReload,
        WeaponData.ReloadDuration,
        false
    );
    NotifyAmmoUpdate();
}

void AWeaponSMG::FinishReload()
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
void AWeaponSMG::StartFire()
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
        &AWeaponSMG::MakeShot,
        WeaponData.FireFrequency,
        true
    );
}
void AWeaponSMG::InitializeWeaponData(FWeaponData* InData)
{
    WeaponData = *InData;
}

void AWeaponSMG::StopFire()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}





void AWeaponSMG::MakeShot()
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

    AActor* MyOwner = GetOwner();
    if (!MyOwner) return;

    APlayerController* PC = Cast<APlayerController>(MyOwner->GetInstigatorController());
    if (!PC) return;

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const float HalfRad = FMath::DegreesToRadians(WeaponData.BulletSpread * 0.5f);
    FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    FVector TraceEnd = ViewLocation + ShotDirection * WeaponData.TraceMaxDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(MyOwner);
    Params.AddIgnoredActor(this);
    Params.bReturnPhysicalMaterial = true;
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

        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor -> IsA(ACharacterZombie::StaticClass()))
        {
            UGameplayStatics::ApplyPointDamage(
                HitActor,
                10.0f,
                GetActorForwardVector(),
                Hit,
                PC,
                this,
                UDamageType::StaticClass());
        }
    }
    NotifyAmmoUpdate();
}

bool AWeaponSMG::IsReloading()
{
    return bIsReloading;
}
void AWeaponSMG::NotifyAmmoUpdate()
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

void AWeaponSMG::BeginPlay()
{
    Super::BeginPlay();

    // 1) DataTable이 할당되어 있는지 확인
    if (WeaponDataTable)
    {
        // 디버그용 메시지
        static const FString ContextString(TEXT("WeaponSMG DataTable Initialization"));

        // 2) “WeaponSMG”라는 행 이름(Row Name)을 기준으로 FWeaponData* 를 찾아온다.
        //    (에디터의 DataTable에서 Row Name이 정확히 "WeaponSMG" 여야 함)
        FWeaponData* FoundRow = WeaponDataTable->FindRow<FWeaponData>(FName(TEXT("WeaponSMG")), ContextString, /*bWarnIfNotFound=*/ true);
        if (FoundRow)
        {
            // 3) InitializeWeaponData에 그대로 포인터를 넘겨준다
            InitializeWeaponData(FoundRow);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[WeaponSMG] WeaponDataTable에서 'WeaponSMG' 행을 찾지 못했습니다."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[WeaponSMG] WeaponDataTable이 에디터에 연결되지 않았습니다."));
    }
}