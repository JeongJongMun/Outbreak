#include "VibrationAbility.h"

#include "Kismet/GameplayStatics.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Util/CameraShake.h"

void UVibrationAbility::ActivateAbility()
{
	Super::ActivateAbility();

	CachedOwner = GetOwnerCharacter();
	CachedOwner->GetWorldTimerManager().SetTimer(
		VibrationTimerHandle,
		this,
		&UVibrationAbility::OnVibrationAbility,
		VibrationInterval,
		true);
}

void UVibrationAbility::DeactivateAbility()
{
	Super::DeactivateAbility();

	CachedOwner->GetWorldTimerManager().ClearTimer(VibrationTimerHandle);
}

void UVibrationAbility::OnVibrationAbility()
{
	const FVector Origin = CachedOwner->GetActorLocation();

	// TODO : Delete DebugSphere
	DrawDebugSphere(
		CachedOwner->GetWorld(),
		Origin,
		VibrationRange,
		24,
		FColor::Yellow,
		false,
		1.0f
	);

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(CachedOwner->GetWorld(), ACharacterPlayer::StaticClass(), Players);

	for (AActor* Actor : Players)
	{
		ACharacterPlayer* Player = Cast<ACharacterPlayer>(Actor);
		if (!Player || Player == CachedOwner) continue;

		const float Distance = FVector::Dist(Player->GetActorLocation(), Origin);
		if (Distance <= VibrationRange)
		{
			UGameplayStatics::ApplyRadialDamage(CachedOwner->GetWorld(), VibrationDamage, Origin, VibrationRange, nullptr, TArray<AActor*>(), CachedOwner, CachedOwner->GetController(), true);

			if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
			{
				PC->ClientStartCameraShake(UCameraShake::StaticClass(), VibrationIntensity);
			}
		}
	}
}
