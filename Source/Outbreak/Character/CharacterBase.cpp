// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Outbreak/Util/Define.h"
#include "Outbreak/Util/EnumHelper.h"
#include "PhysicsEngine/PhysicsAsset.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Pawn
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetHiddenInGame(false);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Game/FPSAnimationPack/Demo/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (DefaultMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DefaultMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load default zombie mesh"), CURRENT_CONTEXT);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Blueprints/ABP/ABP_Move.ABP_Move_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load AnimInstance"), CURRENT_CONTEXT);
	}
}

float ACharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead()) return 0.0f; // 중복 Die 방지
	
	const float DamageAmount = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType((FPointDamageEvent::ClassID)))
	{
		const auto PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		const FHitResult& HitResult = PointDamageEvent->HitInfo;
		const UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();

		const float DamageMultiplier = GetDamageMultiplier(PhysMat->SurfaceType);
		const int32 FinalDamage = FMath::RoundToInt(DamageAmount * DamageMultiplier);
		ApplyDamage(FinalDamage);
		ApplyHitEffects(FinalDamage, PhysMat->SurfaceType);

		return FinalDamage;
	}
	
	ApplyDamage(DamageAmount);
	ApplyHitEffects(DamageAmount);
		
	return DamageAmount;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitCharacterData();
	SetupCollision();
	SetupMovement();
}

void ACharacterBase::InitCharacterData()
{
	// Implement in derived classes
}

void ACharacterBase::SetPhysicalAsset(const ECharacterType InCharacterType, const ECharacterBodyType InBodyType)
{
	const FString BasePath = TEXT("/Script/Engine.PhysicsAsset'/Game/Physics/PhysicsAssets/");
	const FString CharacterTypeString = EnumHelper::EnumToString(InCharacterType);
	const FString BodyTypeString = EnumHelper::EnumToString(InBodyType);
	const FString AssetName = FString::Printf(TEXT("PA_%s_%s"), *CharacterTypeString, *BodyTypeString);

	const FString FullPath = FString::Printf(TEXT("%s%s.%s'"), *BasePath, *AssetName, *AssetName);

	const TObjectPtr<USkeletalMeshComponent> MeshComponent = GetMesh();

	if (const TObjectPtr<UPhysicsAsset> PhysicsAsset = LoadObject<UPhysicsAsset>(nullptr, *FullPath))
	{
		MeshComponent->SetPhysicsAsset(nullptr);
		MeshComponent->SetPhysicsAsset(PhysicsAsset);
		MeshComponent->RecreatePhysicsState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load Physics Asset: %s"), CURRENT_CONTEXT, *FullPath);
	}
}

bool ACharacterBase::IsDead() const
{
	if (CurrentHealth <= 0)
	{
		return true;
	}
	return false;
}

void ACharacterBase::Die()
{
	// TODO : Implement common death logic
}

float ACharacterBase::GetDamageMultiplier(const EPhysicalSurface SurfaceType)
{
	switch (SurfaceType)
	{
	case SurfaceType1: // Head
		return HeadDamageMultiplier;
	case SurfaceType2: // Body  
		return BodyDamageMultiplier;
	case SurfaceType3: // Limbs
		return LimbsDamageMultiplier;
	default:
		return 1.0f;
	}
}

void ACharacterBase::ApplyDamage(int32 DamageAmount)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] %s got damage %d, HP %d -> %d "), CURRENT_CONTEXT, *GetName(), DamageAmount, CurrentHealth, CurrentHealth - DamageAmount);

	if (CurrentExtraHealth > 0)
	{
		CurrentExtraHealth -= DamageAmount;
		if (CurrentExtraHealth < 0)
		{
			DamageAmount = -CurrentExtraHealth;
			CurrentExtraHealth = 0;
		}
	}
	
	CurrentHealth -= DamageAmount;
	if (CurrentHealth < 0)
		CurrentHealth = 0;

	if (IsDead())
	{
		Die();
	}
	else
	{
		// TODO : Implement hit reaction logic (maybe animation)
	}
}

void ACharacterBase::ApplyHitEffects(const int32 DamageAmount, const EPhysicalSurface SurfaceType)
{
	// TODO : Implement hit effects based on surface type and damage amount
	switch (SurfaceType)
	{
		case SurfaceType1: // Head
			break;

		case SurfaceType2: // Body
			break;

		case SurfaceType3: // Limbs
			break;
		
		default:
			break;
	}
}

void ACharacterBase::SetupCollision()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void ACharacterBase::SetupMovement()
{
	auto* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	MovementComp->JumpZVelocity = 700.f;
	MovementComp->AirControl = 0.35f;
	MovementComp->MaxWalkSpeed = 500.f;
	MovementComp->MinAnalogWalkSpeed = 20.f;
	MovementComp->BrakingDecelerationWalking = 2000.f;
}
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{

		FRotator ControlRot = Controller->GetControlRotation();
		FRotator MeshRotation = FRotator(0.0f, ControlRot.Yaw, 0.0f);
		GetMesh()->SetWorldRotation(MeshRotation);
	}
}
