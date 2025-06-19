#include "WeaponSMG.h"
#include "SMGAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/UI/OB_HUD.h"
#include "Net/UnrealNetwork.h"

AWeaponSMG::AWeaponSMG()
{
    AmmoClass = ASMGAmmo::StaticClass();
    
    MuzzleSocketName = TEXT("Muzzle_SMG");

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshObj(TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/SMG02/SK_weapon_SMG_02.SK_weapon_SMG_02"));
    if (WeaponMeshObj.Succeeded())
    {
        WeaponMesh->SetSkeletalMesh(WeaponMeshObj.Object);
    }
    WeaponMesh->SetHiddenInGame(true);
    
    static ConstructorHelpers::FObjectFinder<USoundBase> ShotSoundObj(TEXT("/Game/Sounds/AR_Single.AR_Single"));
    if (ShotSoundObj.Succeeded())
    {
        WeaponData.ShotSound = ShotSoundObj.Object; 
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

void AWeaponSMG::BeginPlay()
{
    Super::BeginPlay();
    if (!HasAuthority())
        return;

    if (WeaponDataTable)
    {
        static const FString ContextString(TEXT("WeaponSMG DataTable Initialization"));

        FWeaponData* FoundRow = WeaponDataTable->FindRow<FWeaponData>(FName(TEXT("WeaponSMG")), ContextString, /*bWarnIfNotFound=*/ true);
        if (FoundRow)
        {
            InitializeWeaponData(FoundRow);
        }
    }
}

void AWeaponSMG::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWeaponSMG, WeaponData);
}

void AWeaponSMG::InitializeWeaponData(FWeaponData* InData)
{
    WeaponData = *InData;
    WeaponData.CurrentAmmo = WeaponData.MagazineCapacity;
}

void AWeaponSMG::Reload()
{
    if (bIsReloading || WeaponData.CurrentAmmo == WeaponData.MagazineCapacity || WeaponData.TotalAmmo <= 0)
        return;

    bIsReloading = true;
    StopFire();

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
    const int32 Needed = WeaponData.MagazineCapacity - WeaponData.CurrentAmmo;
    const int32 ToReload = FMath::Min(Needed, WeaponData.TotalAmmo);

    WeaponData.CurrentAmmo += ToReload;
    WeaponData.TotalAmmo -= ToReload;
    bIsReloading = false;
    
    NotifyAmmoUpdate();
}

void AWeaponSMG::NotifyAmmoUpdate()
{
    const ACharacterPlayer* OwnerCharacter = Cast<ACharacterPlayer>(GetOwner());
    if (!OwnerCharacter) return;

    const APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
    if (!PC) return;

    if (AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD()))
    {
        HUD->DisplayAmmo(WeaponData.CurrentAmmo, WeaponData.TotalAmmo);
    }
}

void AWeaponSMG::PlayLocalEffects()
{
    UGameplayStatics::PlaySound2D(GetWorld(), WeaponData.ShotSound);
    NotifyAmmoUpdate();
    PlayMuzzleEffect();
    ApplyCameraShake();
}

void AWeaponSMG::StartFire()
{
    if (bIsReloading)
    {
        StopFire();
        return;
    }

    if (WeaponData.CurrentAmmo <= 0)
    {
        Reload();
        return;
    }

    if (!HasAuthority())
    {
        ServerStartFire();
        PlayLocalEffects();
    }
}

void AWeaponSMG::StopFire()
{
    ServerStopFire();
}

void AWeaponSMG::ServerStartFire_Implementation()
{
    if (bIsReloading || WeaponData.CurrentAmmo <= 0)
        return;

    MakeShot();
    
    GetWorldTimerManager().SetTimer(
        TimerHandle_TimeBetweenShots,
        this,
        &AWeaponSMG::MakeShot,
        WeaponData.FireFrequency,
        true
    );
}

void AWeaponSMG::MakeShot()
{
    WeaponData.CurrentAmmo--;

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

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, ECC_Visibility, Params);
    
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
        if (HitActor && HitActor->IsA(ACharacterZombie::StaticClass()))
        {
            UGameplayStatics::ApplyPointDamage(
                HitActor,
                40.0f,
                GetActorForwardVector(),
                Hit,
                PC,
                this,
                UDamageType::StaticClass());
        }
    }
}

void AWeaponSMG::ServerStopFire_Implementation()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AWeaponSMG::MultiCastShot_Implementation(AController* ShooterController)
{
    APlayerController* LocalPC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (LocalPC == ShooterController)
        return;
    
    UGameplayStatics::PlaySound2D(GetWorld(), WeaponData.ShotSound);
}

void AWeaponSMG::PlayMuzzleEffect()
{
    FRotator MuzzleRot = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    UNiagaraFunctionLibrary::SpawnSystemAttached(
        NiagaraMuzzleFlash, 
        WeaponMesh,                     
        TEXT("Muzzle_SMG"),
        FVector::ZeroVector,
        FRotator(MuzzleRot.Pitch, MuzzleRot.Yaw + 90.0f, MuzzleRot.Roll),
        EAttachLocation::SnapToTarget,
        true
    );
}