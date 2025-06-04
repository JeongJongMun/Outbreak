// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
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
	GetCharacterMovement()->bOrientRotationToMovement = true;
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ZombieMegaBundle/Mesh_UE5/UE5_Skeleton/Body/SKM_Body_Afro_UE5.SKM_Body_Afro_UE5'"));
	if (DefaultMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DefaultMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load default zombie mesh"), CURRENT_CONTEXT);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load AnimInstance"), CURRENT_CONTEXT);
	}
}

void ACharacterBase::TakeHitDamage(const FHitResult& HitResult, const int32 BaseDamage)
{
	const UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();
    
	if (!PhysMat)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] %s PhysMaterial is null, applying base damage: %d"), CURRENT_CONTEXT, *GetName(), BaseDamage);
		ApplyDamage(BaseDamage);
		// TODO : Apply default hit effects
		return;
	}
    
	EPhysicalSurface SurfaceType = PhysMat->SurfaceType;
	const float DamageMultiplier = GetDamageMultiplier(SurfaceType);
	const int32 FinalDamage = FMath::RoundToInt(BaseDamage * DamageMultiplier);
    
	ApplyDamage(FinalDamage);
	ApplyHitEffects(SurfaceType, FinalDamage);
	
	UE_LOG(LogTemp, Log, TEXT("[%s] On %s, BaseDamage: %d, SurfaceType: %d, FinalDamage: %d"), *GetName(), CURRENT_CONTEXT, BaseDamage, SurfaceType, FinalDamage);
}

void ACharacterBase::SetPhysicalAsset(const ECharacterType CharacterType, const ECharacterBodyType BodyType)
{
	const FString BasePath = TEXT("/Script/Engine.PhysicsAsset'/Game/Physics/PhysicsAssets/");
	const FString CharacterTypeString = EnumHelper::EnumToString(CharacterType);
	const FString BodyTypeString = EnumHelper::EnumToString(BodyType);
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

	if (IsDead())
	{
		Die();
	}
	else
	{
		// TODO : Implement hit reaction logic (maybe animation)
	}
}

void ACharacterBase::ApplyHitEffects(const EPhysicalSurface SurfaceType, int32 DamageAmount)
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
