// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterPlayer.h"
#include "PlayerControlData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterPlayer::ACharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	auto CM = GetCharacterMovement();
	CM->bUseControllerDesiredRotation = true;
	CM->bOrientRotationToMovement = true;

	// ControlData Map
	static ConstructorHelpers::FObjectFinder<UPlayerControlData> ShoulderDataRef(TEXT("/Script/Outbreak.PlayerControlData'/Game/Data/DA_Shoulder.DA_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		PlayerControlMap.Add(EPlayerControlType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UPlayerControlData> TopDataRef(TEXT("/Script/Outbreak.CharacterControlData'/Game/Data/DA_Top.DA_Top'"));
	if (TopDataRef.Object)
	{
		PlayerControlMap.Add(EPlayerControlType::Top, TopDataRef.Object);
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuarterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_TopMove.IA_TopMove'"));
	if (InputActionQuarterMoveRef.Object)
	{
		TopMoveAction = InputActionQuarterMoveRef.Object;
	}
	
	CurrentCharacterControlType = EPlayerControlType::Top;
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetCharacterControl(CurrentCharacterControlType);
}

void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(TopMoveAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::TopMove);
}

void ACharacterPlayer::SetCharacterControlData(const class UPlayerControlData* ControlData)
{
	// Pawn
	bUseControllerRotationYaw = ControlData->bUseControllerRotationYaw;

	// Character Movement
	auto CM = GetCharacterMovement();
	CM->bOrientRotationToMovement = ControlData->bOrientRotationToMovement;
	CM->bUseControllerDesiredRotation = ControlData->bUseControllerDesiredRotation;
	CM->RotationRate = ControlData->RotationRate;

	// Camera
	CameraBoom->TargetArmLength = ControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(ControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = ControlData->bUsePawnControlRotation;
	CameraBoom->bInheritYaw = ControlData->bInheritYaw;
	CameraBoom->bInheritRoll = ControlData->bInheritRoll;
	CameraBoom->bInheritPitch = ControlData->bInheritPitch;
	CameraBoom->bDoCollisionTest = ControlData->bDoCollisionTest;
}

void ACharacterPlayer::InitializePlayerData(FPlayerData* InData)
{
	PlayerData = *InData;
	CurrentHealth = PlayerData.MaxHealth;
	CurrentExtraHealth = 0;
}

void ACharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == EPlayerControlType::Top)
	{
		SetCharacterControl(EPlayerControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == EPlayerControlType::Shoulder)
	{
		SetCharacterControl(EPlayerControlType::Top);
	}
}

void ACharacterPlayer::SetCharacterControl(EPlayerControlType NewCharacterControlType)
{
	UPlayerControlData* NewCharacterControlData = PlayerControlMap[NewCharacterControlType];
	check(NewCharacterControlData);
	SetCharacterControlData(NewCharacterControlData);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControlData->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	CurrentCharacterControlType = NewCharacterControlType;
}

void ACharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ACharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACharacterPlayer::TopMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}
