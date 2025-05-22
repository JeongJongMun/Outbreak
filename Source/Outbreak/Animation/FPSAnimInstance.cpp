#include "FPSAnimInstance.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn) return;

	ACharacterPlayer* Character = Cast<ACharacterPlayer>(OwningPawn);
	if (!Character) return;

	FVector Velocity = Character->GetVelocity();
	Speed = Velocity.Size();

	FVector Forward = Character->GetActorRotation().Vector();
	FVector MovementDir = Velocity.GetSafeNormal2D();

	float ForwardDot = FVector::DotProduct(Forward, MovementDir);
	float RightDot = FVector::DotProduct(FRotationMatrix(Character->GetActorRotation()).GetScaledAxis(EAxis::Y), MovementDir);
	float Angle = FMath::Atan2(RightDot, ForwardDot);

	Direction = FMath::RadiansToDegrees(Angle);
	bIsSprinting = Character->IsSprinting();
	bIsCrouching = Character->IsCrouching();

}
