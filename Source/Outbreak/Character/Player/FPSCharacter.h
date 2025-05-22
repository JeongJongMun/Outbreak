#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Outbreak/Weapon/WeaponBase.h"
#include "FPSCharacter.generated.h"

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
    FPS,
    TopView
};

UCLASS()
class OUTBREAK_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	bool IsCrouching() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	bool IsSprinting() const;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	FTransform GetMuzzleTransform() const;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	// 입력 처리
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 입력 액션
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponClass; // 에디터에서 디버깅용으로 할당하는 무기 클래스

	UPROPERTY(EditDefaultsOnly,Category= "BulletFactory")
	TSubclassOf<AAmmoBase> bulletFactory;

	UPROPERTY()
	AWeaponBase* CurrentWeapon;   // 실제 런타임상에서 변경되는 무기 클래스
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_Sprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_Crouch;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_ChangeCam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_Fire;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction_ChageFireMode;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed = 200.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching = false;

	UFUNCTION()
	void StartSprinting();

	UFUNCTION()
	void StopSprinting();

	UFUNCTION()
	void BeginCrouch();

	UFUNCTION()
	void EndCrouch();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TopViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	ECameraMode CurrentCameraMode = ECameraMode::FPS;

	UPROPERTY(VisibleAnywhere, Category="Mesh")
	USkeletalMeshComponent* FirstPersonMesh;


	UPROPERTY(VisibleAnywhere, Category="Mesh")
	USkeletalMeshComponent* GunMesh;
	
	UFUNCTION()
	void ToggleCameraMode();

	UFUNCTION()
	void OnFirePressed();

	UFUNCTION()
	void OnFireReleased();
	
	UFUNCTION()
	void OnToggleFireMode();
	
	bool bIsAutoFire = false;

};
