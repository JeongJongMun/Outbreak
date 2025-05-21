// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "DrawDebugHelpers.h"  // 맨 위에 추가
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera -> SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera -> SetRelativeLocation(FVector(20, 0, BaseEyeHeight));
	FirstPersonCamera -> bUsePawnControlRotation = true;

	TopViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopViewCamera"));
	TopViewCamera->SetupAttachment(RootComponent);
	TopViewCamera->SetRelativeLocation(FVector(0.f, 0.f, 800.f));
	TopViewCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TopViewCamera->bUsePawnControlRotation = false;


	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;



	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;


	
	
}
FTransform AFPSCharacter::GetMuzzleTransform() const
{
	if (CurrentWeapon)
	{
		USkeletalMeshComponent* WeapMesh = CurrentWeapon->GetWeaponMesh();
		FName SocketName = CurrentWeapon->GetMuzzleSocketName();

		if (WeapMesh && WeapMesh->DoesSocketExist(SocketName))
		{
			return WeapMesh->GetSocketTransform(SocketName, RTS_World);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon Not Detected"));
	}
	return GetActorTransform();
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(InputMapping, 0);

			}
		}
	}
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

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		EnhancedInput->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		EnhancedInput->BindAction(InputAction_Jump, ETriggerEvent::Started, this, &AFPSCharacter::Jump);
		EnhancedInput->BindAction(InputAction_Jump, ETriggerEvent::Completed, this, &AFPSCharacter::StopJumping);
		EnhancedInput->BindAction(InputAction_Sprint, ETriggerEvent::Started, this, &AFPSCharacter::StartSprinting);
		EnhancedInput->BindAction(InputAction_Sprint, ETriggerEvent::Completed, this, &AFPSCharacter::StopSprinting);;
		EnhancedInput->BindAction(InputAction_Crouch, ETriggerEvent::Started, this, &AFPSCharacter::BeginCrouch);
		EnhancedInput->BindAction(InputAction_Crouch, ETriggerEvent::Completed, this, &AFPSCharacter::EndCrouch);
		EnhancedInput->BindAction(InputAction_ChangeCam, ETriggerEvent::Triggered, this, &AFPSCharacter::ToggleCameraMode);
		EnhancedInput->BindAction(InputAction_Fire, ETriggerEvent::Started, this, &AFPSCharacter::OnFirePressed);
		EnhancedInput->BindAction(InputAction_Fire, ETriggerEvent::Completed, this, &AFPSCharacter::OnFireReleased);
		EnhancedInput->BindAction(InputAction_ChageFireMode, ETriggerEvent::Completed, this, &AFPSCharacter::OnToggleFireMode);
	}
	
}


void AFPSCharacter::Move(const FInputActionValue& Value)
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


void AFPSCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}
void AFPSCharacter::StartSprinting()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFPSCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void AFPSCharacter::BeginCrouch()
{
	bIsCrouching = true;
	GetCharacterMovement() -> MaxWalkSpeed = CrouchSpeed;
}

void AFPSCharacter::EndCrouch()
{
	bIsCrouching = false;
	GetCharacterMovement() -> MaxWalkSpeed = WalkSpeed;
}

bool AFPSCharacter::IsCrouching() const
{
	return bIsCrouching;
}

bool AFPSCharacter::IsSprinting() const
{
	return bIsSprinting; 
}

void AFPSCharacter::ToggleCameraMode()
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




void AFPSCharacter::OnFirePressed()
{
	if (!CurrentWeapon) return;

	if (bIsAutoFire)
	{
		// 연사 모드: 눌러있는 동안 StartFire → StopFire 는 Released 에서
		CurrentWeapon->StartFire();
	}
	else
	{
		// 단발 모드: 누르자마자 한 발만
		CurrentWeapon->StartFire();
		CurrentWeapon->StopFire();
	}
}


void AFPSCharacter::OnFireReleased()
{
	if (bIsAutoFire && CurrentWeapon)
	{
		// 연사 모드에서만 Release 시에 멈춤
		CurrentWeapon->StopFire();
	}
}
void AFPSCharacter::OnToggleFireMode()
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








