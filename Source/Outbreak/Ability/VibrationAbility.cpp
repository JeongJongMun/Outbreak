#include "VibrationAbility.h"

#include "Kismet/GameplayStatics.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Util/CameraShake.h"

void UVibrationAbility::ActivateAbility()
{
	Super::ActivateAbility();

	GetOwner()->GetWorldTimerManager().SetTimer(
		VibrationTimerHandle,
		this,
		&UVibrationAbility::OnVibrationAbility,
		VibrationInterval,
		true);
}

void UVibrationAbility::DeactivateAbility()
{
	Super::DeactivateAbility();

	GetOwner()->GetWorldTimerManager().ClearTimer(VibrationTimerHandle);
}

void UVibrationAbility::OnVibrationAbility()
{
	const FVector Origin = GetOwner()->GetActorLocation();

	// TODO : Delete DebugSphere
	DrawDebugSphere(
		GetOwner()->GetWorld(),
		Origin,
		VibrationRange,
		24,
		FColor::Yellow,
		false,
		1.0f
	);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetOwner()->GetWorld(), ACharacterPlayer::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		ACharacterPlayer* Player = Cast<ACharacterPlayer>(Actor);
		if (!Player || Player == GetOwner()) continue;

		const float Distance = FVector::Dist(Player->GetActorLocation(), Origin);
		if (Distance <= VibrationRange)
		{
			UGameplayStatics::ApplyDamage(Player, VibrationDamage, GetOwner()->GetController(), GetOwner(), UDamageType::StaticClass());

			if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
			{
				PC->ClientStartCameraShake(UCameraShake::StaticClass(), VibrationIntensity);
			}
		}
	}
}
