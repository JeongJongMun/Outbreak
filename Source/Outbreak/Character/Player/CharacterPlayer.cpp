// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Outbreak/Animation/FPSAnimInstance.h"
#include "Outbreak/Weapon/WeaponAR.h"

ACharacterPlayer::ACharacterPlayer()
{
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera -> SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera -> SetRelativeLocation(FVector(20, 0, BaseEyeHeight));
	FirstPersonCamera -> bUsePawnControlRotation = true;

	TopViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopViewCamera"));
	TopViewCamera->SetupAttachment(RootComponent);
	TopViewCamera->SetRelativeLocation(FVector(0.f, 0.f, 800.f));
	TopViewCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TopViewCamera->bUsePawnControlRotation = false;

	GetMesh()->SetHiddenInGame(true);
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FirstPersonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Pack/SkeletalMeshes/Arms/SK_fps_armRig.SK_fps_armRig'"));
	if (FirstPersonMeshRef.Succeeded())
	{
		FirstPersonMesh->SetSkeletalMesh(FirstPersonMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UFPSAnimInstance> FPSAnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/FPS_Weapon_Pack/Animation/Arms/AR02/ABP_Arms_AR02.ABP_Arms_AR02_C'"));
	if (FPSAnimInstanceRef.Succeeded())
	{
		FirstPersonMesh->SetAnimInstanceClass(FPSAnimInstanceRef.Class);
	}
	
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	static ConstructorHelpers::FClassFinder<UFPSAnimInstance> GunAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/FPS_Weapon_Pack/Animation/AR02/ABP_AR02.ABP_AR02_C'"));
	if (GunAnimInstance.Succeeded())
	{
		GunMesh->SetAnimInstanceClass(GunAnimInstance.Class);
	}
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	WeaponClass = AWeaponAR::StaticClass();

	auto CM = GetCharacterMovement();
	CM->bUseControllerDesiredRotation = true;
	CM->bOrientRotationToMovement = true;

	// Input Mapping Context
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Player.IMC_Player'"));
	if (InputMappingContextRef.Object)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Sprint.IA_Sprint'"));
	if (InputActionSprintRef.Object)
	{
		SprintAction = InputActionSprintRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Crouch.IA_Crouch'"));
	if (InputActionCrouchRef.Object)
	{
		CrouchAction = InputActionCrouchRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeCamRef(TEXT("/Script/EnhancedInputComponent.InputAction'/Game/Inputs/IA_ChangePerspective.IA_ChangePerspective'"));
	if (InputActionChangeCamRef.Object)
	{
		ChangeCameraAction = InputActionChangeCamRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFireRef(TEXT("/Script/EnhancedInputComponent.InputAction'/Game/Inputs/IA_Fire.IA_Fire'"));
	if (InputActionFireRef.Object)
	{
		FireAction = InputActionFireRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeFireModeRef(TEXT("/Script/EnhancedInputComponent.InputAction'/Game/Inputs/IA_ToggleFireMode.IA_ToggleFireMode'"));
	if (InputActionChangeFireModeRef.Object)
	{
		ChangeFireModeAction = InputActionChangeFireModeRef.Object;
	}
	
	CurrentCharacterControlType = EPlayerControlType::Top;
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetCharacterControl(CurrentCharacterControlType);
	if (FirstPersonCamera)
	{
		FirstPersonCamera->SetActive(true);
	}
	if (TopViewCamera)
	{
		TopViewCamera->SetActive(false);
	}

	if (WeaponClass)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(
			WeaponClass,
			FVector::ZeroVector, FRotator::ZeroRotator);

		if (CurrentWeapon)
		{

			CurrentWeapon->AttachToComponent(
				FirstPersonMesh, 
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("weapon_l_Socket"));  
		}
	}
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->SetInstigator(this);
}

void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacterPlayer::StopJumping);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACharacterPlayer::StartSprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACharacterPlayer::StopSprinting);;
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACharacterPlayer::BeginCrouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacterPlayer::EndCrouch);
	
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACharacterPlayer::OnFirePressed);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACharacterPlayer::OnFireReleased);
	
	EnhancedInputComponent->BindAction(ChangeFireModeAction, ETriggerEvent::Completed, this, &ACharacterPlayer::OnToggleFireMode);
	EnhancedInputComponent->BindAction(ChangeCameraAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::ToggleCameraMode);
}

void ACharacterPlayer::ToggleCameraMode()
{
	if (CurrentCameraMode == ECameraMode::FPS)
	{
		CurrentCameraMode = ECameraMode::TopView;
		FirstPersonCamera->SetActive(false);
		TopViewCamera->SetActive(true);
	}
	else
	{
		CurrentCameraMode = ECameraMode::FPS;
		TopViewCamera->SetActive(false);
		FirstPersonCamera->SetActive(true);
	}
}

void ACharacterPlayer::OnFirePressed()
{
	if (!CurrentWeapon) return;

	if (bIsAutoFire)
	{
		CurrentWeapon->StartFire();
	}
	else
	{
		CurrentWeapon->StartFire();
		CurrentWeapon->StopFire();
	}
}

void ACharacterPlayer::OnFireReleased()
{
	if (bIsAutoFire && CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ACharacterPlayer::OnToggleFireMode()
{
	bIsAutoFire = !bIsAutoFire;

	if (GEngine)
	{
		const TCHAR* ModeText = bIsAutoFire ? TEXT("Auto Fire") : TEXT("Single Fire");
		GEngine->AddOnScreenDebugMessage(
			-1, 2.5f, FColor::Yellow,
			FString::Printf(TEXT("Fire Mode: %s"), ModeText)
		);
	}
}

void ACharacterPlayer::InitializePlayerData(FPlayerData* InData)
{
	PlayerData = *InData;
	CurrentHealth = PlayerData.MaxHealth;
	CurrentExtraHealth = 0;
}

void ACharacterPlayer::SetCharacterControl(EPlayerControlType NewCharacterControlType)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	CurrentCharacterControlType = NewCharacterControlType;
}

void ACharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	    
	if (Controller && MovementVector != FVector2D::ZeroVector)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);  // W/S
		AddMovementInput(RightDirection, MovementVector.X);    // A/D
	}
}

void ACharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}
void ACharacterPlayer::StartSprinting()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACharacterPlayer::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void ACharacterPlayer::BeginCrouch()
{
	bIsCrouching = true;
	GetCharacterMovement() -> MaxWalkSpeed = CrouchSpeed;
}

void ACharacterPlayer::EndCrouch()
{
	bIsCrouching = false;
	GetCharacterMovement() -> MaxWalkSpeed = WalkSpeed;
}

bool ACharacterPlayer::IsCrouching() const
{
	return bIsCrouching;
}

bool ACharacterPlayer::IsSprinting() const
{
	return bIsSprinting; 
}