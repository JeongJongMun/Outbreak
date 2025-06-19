// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"

#include "Kismet/GameplayStatics.h"

void USoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USoundManager::Deinitialize()
{
	Super::Deinitialize();
}

void USoundManager::PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float VolumeMultiplier,
	float PitchMultiplier)
{
	if (GetWorld()->IsNetMode(NM_DedicatedServer) || !Sound)
		return;
        
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, SFXVolume * VolumeMultiplier, PitchMultiplier);
}

void USoundManager::PlaySound2D(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier)
{
	if (GetWorld()->IsNetMode(NM_DedicatedServer) || !Sound)
		return;
        
	UGameplayStatics::PlaySound2D(GetWorld(), Sound, SFXVolume * VolumeMultiplier, PitchMultiplier);
}

void USoundManager::StopAllSounds()
{

}

void USoundManager::SetBGMVolume(const float InVolume)
{
	BGMVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
}

void USoundManager::SetSFXVolume(const float InVolume)
{
	SFXVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
}
