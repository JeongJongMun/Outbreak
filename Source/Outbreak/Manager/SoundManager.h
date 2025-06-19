// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManager.generated.h"

/**
if (UAudioManager* AudioManager = GetGameInstance()->GetSubsystem<UAudioManager>())
{
	AudioManager->PlaySoundAtLocation(YourSound, GetActorLocation());
}
 */
UCLASS()
class OUTBREAK_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);
	void PlaySound2D(USoundBase* Sound, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);
	void StopAllSounds();
	void SetBGMVolume(float InVolume);
	float GetBgmVolume() const { return BGMVolume; }
	void SetSFXVolume(float InVolume);
	float GetSFXVolume() const { return SFXVolume; }

private:
	float BGMVolume = 0.5f;
	float SFXVolume = 0.5f;
	
};
