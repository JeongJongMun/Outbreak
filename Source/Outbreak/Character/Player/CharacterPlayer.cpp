// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "PaperSprite.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Outbreak/Character/Zombie/CharacterSpawnManager.h"
#include "Outbreak/Game/OutBreakGameState.h"
#include "Outbreak/Game/OutBreakPlayerState.h"
#include "Outbreak/UI/OB_HUD.h"
#include "Outbreak/Weapon/WeaponAR.h"
#include "Outbreak/Weapon/WeaponSMG.h"

ACharacterPlayer::ACharacterPlayer()
{
	CharacterType = ECharacterType::Player;
	PlayerType = EPlayerType::Player1;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera -> SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera -> SetRelativeLocation(FVector(20, 0, BaseEyeHeight));
	FirstPersonCamera -> bUsePawnControlRotation = true;

	TopViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopViewCamera"));
	TopViewCamera->SetupAttachment(RootComponent);
	TopViewCamera->SetRelativeLocation(FVector(0.f, 0.f, 800.f));
	TopViewCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TopViewCamera->bUsePawnControlRotation = false;
	
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
	SceneCapture->ProjectionType = ECameraProjectionMode::Type::Orthographic;
	SceneCapture->OrthoWidth = 4000.f;
	SceneCapture->SetupAttachment(RootComponent); 
	SceneCapture->SetRelativeLocation(FVector(0.f, 0.f, 2100.f)); 
	SceneCapture->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); 
	SceneCapture->bCaptureEveryFrame = true;
	SceneCapture->bCaptureOnMovement = false;
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetRef(TEXT("/Game/UI/MiniMap/RT_Minimap.RT_Minimap"));
	if (RenderTargetRef.Succeeded())
	{
		SceneCapture->TextureTarget = RenderTargetRef.Object;
	}

	PlayerIconSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerIconSprite"));
	PlayerIconSprite->SetupAttachment(GetCapsuleComponent());
	PlayerIconSprite->SetRelativeLocation(FVector(0.f, 0.f, 2000.f));
	PlayerIconSprite->SetRelativeRotation(FRotator(-180.f, -180.f, -90.f));
	PlayerIconSprite->SetRelativeScale3D(FVector(0.5f));       
	PlayerIconSprite->SetVisibility(true);
	PlayerIconSprite->bVisibleInSceneCaptureOnly = true;
	static ConstructorHelpers::FObjectFinder<UPaperSprite> PlayerIconAsset(TEXT("/Game/UI/MiniMap/PlayerIcon_Sprite.PlayerIcon_Sprite"));
	if (PlayerIconAsset.Succeeded())
	{
		PlayerIconSprite->SetSprite(PlayerIconAsset.Object);
	}
	
	PlayerNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerNameText"));
	PlayerNameText->SetupAttachment(GetCapsuleComponent());
	PlayerNameText->SetRelativeLocation(FVector(-250.f, 0.f, 2000.f));
	PlayerNameText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	PlayerNameText->SetHorizontalAlignment(EHTA_Center);
	PlayerNameText->SetVerticalAlignment(EVRTA_TextCenter);
	PlayerNameText->SetWorldSize(200.f); 
	PlayerNameText->SetTextRenderColor(FColor::White);
	PlayerNameText->SetVisibility(true);
	PlayerNameText->bVisibleInSceneCaptureOnly = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Game/FPSAnimationPack/Demo/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (DefaultMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DefaultMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Blueprints/ABP/ABP_Move.ABP_Move_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FirstPersonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Pack/SkeletalMeshes/Arms/SK_fps_armRig.SK_fps_armRig'"));
	if (FirstPersonMeshRef.Succeeded())
	{
		FirstPersonMesh->SetSkeletalMesh(FirstPersonMeshRef.Object);
	}
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetRelativeLocation(FVector(20.f, 10.f, -20.f));
	FirstPersonMesh->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(
		FirstPersonMesh,
		TEXT("weapon_socket_l"));
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Reload.IA_Reload'"));
	if (InputActionReloadRef.Object)
	{
		ReloadAction = InputActionReloadRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSwap1(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_SwapSlot1.IA_SwapSlot1'"));
	if (InputActionReloadRef.Object)
	{
		SwapSlot1 = InputActionSwap1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSwap2(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_SwapSlot2.IA_SwapSlot2'"));
	if (InputActionReloadRef.Object)
	{
		SwapSlot2 = InputActionSwap2.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SMGMeshobj(
	TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/SMG02/SK_weapon_SMG_02.SK_weapon_SMG_02"));
	if (SMGMeshobj.Object)
	{
		SMGMesh = SMGMeshobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ARMeshObj(
	TEXT("/Game/FPS_Weapon_Pack/SkeletalMeshes/AR2/SM_weapon_AR2.SM_weapon_AR2"));
	if (ARMeshObj.Object)
	{
		ARMesh = ARMeshObj.Object;
	}
	
	CurrentCharacterControlType = EPlayerControlType::Top;
	WeaponInventory.SetNum(6);
	WeaponInstances.SetNum(6);
	CurrentWeapon = nullptr;
	CurrentSlotIndex = -1;
}

void ACharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterPlayer, PlayerData);
	DOREPLIFETIME(ACharacterPlayer, PlayerType);
}

void ACharacterPlayer::InitCharacterData()
{
	Super::InitCharacterData();
	
	if (HasAuthority())
	{
		const AOutBreakGameState* GameState = Cast<AOutBreakGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (!GameState)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] GameState is null!"), CURRENT_CONTEXT);
			return;
		}
		ACharacterSpawnManager* SpawnManager = GameState->GetSpawnManager();
		if (!SpawnManager)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] SpawnManager is null!"), CURRENT_CONTEXT);
			return;
		}

		const FPlayerData* Data = SpawnManager->GetPlayerData(PlayerType);
		PlayerData = *Data;
	}
	
	CurrentHealth = PlayerData.MaxHealth;
	CurrentExtraHealth = 0;
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetOwnerNoSee(true);
	if (IsLocallyControlled())
	{
		SetCharacterControl(CurrentCharacterControlType);
		if (FirstPersonCamera)
		{
			FirstPersonCamera->SetActive(true);
		}
		if (TopViewCamera)
		{
			TopViewCamera->SetActive(false);
		}
	}
	else
	{
		GunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("weapon_socket_TP"));
	}
	AOutBreakPlayerState* PS = Cast<AOutBreakPlayerState>(GetPlayerState());
	if (PS && PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(PS->PlayerNickname));
	}
	
	/// 인벤토리 스왑 디버깅용 코드
	WeaponInventory[0] = AWeaponAR::StaticClass();
	WeaponInventory[1] = AWeaponSMG::StaticClass();
	/// 
	for (int32 i = 0; i < WeaponInventory.Num(); ++i)
	{
		if (WeaponInventory[i])
		{

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.bDeferConstruction = true;  
			// 월드에 무기 액터 스폰
			AWeaponBase* NewWeapon = GetWorld()->SpawnActorDeferred<AWeaponBase>(
				WeaponInventory[i],
				FTransform::Identity,
				SpawnParams.Owner,
				SpawnParams.Instigator,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			if (NewWeapon)
			{
				// 처음에는 모두 비활성 상태로 두거나, 보이지 않게 설정
				NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("weapon_socket_l"))); 
				NewWeapon->SetActorHiddenInGame(true);
				NewWeapon->SetActorEnableCollision(false);

				UGameplayStatics::FinishSpawningActor(NewWeapon, FTransform::Identity);
				
				WeaponInstances[i] = NewWeapon;
			}
		}
		else
		{
			WeaponInstances[i] = nullptr;
		}
	}
	SwapToSlot(1);
	ChangeArm();
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

	EnhancedInputComponent->BindAction(ReloadAction,ETriggerEvent::Triggered,this,&ACharacterPlayer::OnReload);
	EnhancedInputComponent->BindAction(SwapSlot1,ETriggerEvent::Triggered,this,&ACharacterPlayer::OnPressedSlot1);
	EnhancedInputComponent->BindAction(SwapSlot2,ETriggerEvent::Triggered,this,&ACharacterPlayer::OnPressedSlot2);
}

void ACharacterPlayer::OnDie()
{
	Super::OnDie();
	
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
	}
	
	DetachFromControllerPendingDestroy();
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
void ACharacterPlayer::OnReload()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
	FString::Printf(TEXT("Reload Pressed")));
	CurrentWeapon->Reload();
}

void ACharacterPlayer::OnFirePressed()
{
	if (!CurrentWeapon || bIsCutscenePlaying) return;

	if (bIsAutoFire)
	{
		bIsShooting = true;
		CurrentWeapon->StartFire();
	}
	else
	{
		bIsShooting = true;
		CurrentWeapon->StartFire();
		bIsShooting = false;
		CurrentWeapon->StopFire();
	}
}

void ACharacterPlayer::OnFireReleased()
{
	if (bIsAutoFire && CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		bIsShooting = false;
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

void ACharacterPlayer::ChangeArm()
{
	UClass* ArmAnimClass = nullptr;
	UClass* WeaponAnimClass = nullptr;
	
	if (CurrentWeapon->GetClass() == AWeaponAR::StaticClass())
	{
		ArmAnimClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, TEXT("/Game/FPS_Weapon_Pack/Animation/Arms/AR02/ABP_Arms_AR02.ABP_Arms_AR02_C"));
		WeaponAnimClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, TEXT("/Game/FPS_Weapon_Pack/Animation/AR02/ABP_AR02.ABP_AR02_C"));
		GunMesh->SetSkeletalMesh(ARMesh);
		
	}
	else if (CurrentWeapon->GetClass() == AWeaponSMG::StaticClass())
	{
		ArmAnimClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, TEXT("/Game/FPS_Weapon_Pack/Animation/Arms/MP2/ABP_Arms_MP2.ABP_Arms_MP2_C"));
		WeaponAnimClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, TEXT("/Game/FPS_Weapon_Pack/Animation/SMG02/ABP_SMG02.ABP_SMG02_C"));
		GunMesh->SetSkeletalMesh(SMGMesh);
	}
	else
	{
		ArmAnimClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, TEXT("/Game/FPS_Weapon_Pack/Animation/Arms/MP2/ABP_Arms_MP2.ABP_Arms_MP2_C"));
		WeaponAnimClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, TEXT("/Game/FPS_Weapon_Pack/Animation/SMG02/ABP_SMG02.ABP_SMG02_C"));
	}
	if (CurrentWeapon)
			CurrentWeapon->AttachToComponent(
				FirstPersonMesh, 
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("weapon_l_Socket"));
	if (ArmAnimClass)
	{
		FirstPersonMesh->SetAnimInstanceClass(ArmAnimClass);
	}

	if (WeaponAnimClass)
	{
		GunMesh->SetAnimInstanceClass(WeaponAnimClass);
	}

}

void ACharacterPlayer::SwapToSlot(int32 NewSlotIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 2.5f, FColor::Yellow,
			FString::Printf(TEXT("Selected Slot: %d"), NewSlotIndex)
		);
	}
	
	if (NewSlotIndex < 0 || NewSlotIndex >= WeaponInstances.Num()) return;
	if (CurrentSlotIndex == NewSlotIndex) return;
	
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
	}
	
	AWeaponBase* NewWeapon = WeaponInstances[NewSlotIndex];
	if (NewWeapon)
	{
		NewWeapon->SetActorHiddenInGame(false);
		NewWeapon->SetActorEnableCollision(true);
	}

	CurrentWeapon = NewWeapon;
	CurrentSlotIndex = NewSlotIndex;
	GEngine->AddOnScreenDebugMessage(
		-1, 2.5f, FColor::Yellow,
		FString::Printf(TEXT("Selected Slot: %d"), CurrentWeapon)
	);

	ChangeArm();
	if (CurrentWeapon)
	{
		CurrentWeapon->NotifyAmmoUpdate();

		FString WeaponType;
		if (NewSlotIndex == 0) WeaponType = "AR";
		else if (NewSlotIndex == 1) WeaponType = "SMG";
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD());
			if (HUD)
			{
				HUD->DisplayWeaponType(WeaponType);
			}
		}
	}
}

void ACharacterPlayer::OnPressedSlot1()
{
	SwapToSlot(0);
}

void ACharacterPlayer::OnPressedSlot2()
{
	SwapToSlot(1);
}

void ACharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	FRotator CameraRotation = FirstPersonCamera->GetComponentRotation();
	FRotator TargetRotation = FRotator(0.f, CameraRotation.Yaw, 0.f);
	GetMesh()->SetWorldRotation(TargetRotation);
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

bool ACharacterPlayer::IsShooting() const
{
	return bIsShooting;
}

bool ACharacterPlayer::GetFireMode() const
{
	return bIsAutoFire;
}

bool ACharacterPlayer::IsReloading() const
{
	if (!CurrentWeapon) return false;
	return CurrentWeapon -> bIsReloading;
}

void ACharacterPlayer::SetupCollision()
{
	Super::SetupCollision();

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	auto* MeshComp = GetMesh();
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComp->bOwnerNoSee = true;
	// MeshComp->SetHiddenInGame(true);
}

void ACharacterPlayer::SetupMovement()
{
	Super::SetupMovement();

	auto* MovementComp = GetCharacterMovement();
	MovementComp->MaxStepHeight = 50.f;
	MovementComp->SetWalkableFloorAngle(55.f);
	MovementComp->bUseControllerDesiredRotation = true;
}
