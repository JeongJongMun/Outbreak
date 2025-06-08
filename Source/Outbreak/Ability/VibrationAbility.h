#pragma once
#include "PassiveAbilityObject.h"
#include "VibrationAbility.generated.h"

UCLASS()
class OUTBREAK_API UVibrationAbility : public UPassiveAbilityObject
{
	GENERATED_BODY()

public:
	UVibrationAbility()
	{
		AbilityType = EAbilityType::Vibration;
		AbilityName = TEXT("Vibration");
	}

	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

private:
	void OnVibrationAbility();

private:
	TObjectPtr<ACharacterBase> CachedOwner;
	FTimerHandle VibrationTimerHandle;
	float VibrationInterval = 1.0f;
	float VibrationDamage = 5.0f;
	float VibrationIntensity = 0.5f;
	float VibrationRange = 1000.0f;
};
