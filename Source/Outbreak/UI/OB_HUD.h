// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Outbreak/Util/Define.h"
#include "OB_HUD.generated.h"

UCLASS()
class OUTBREAK_API AOB_HUD : public AHUD
{
	GENERATED_BODY()


public:
	AOB_HUD();
	virtual void BeginPlay() override;
	
	void DisplayAlivePlayerCount(int32 AlivePlayerCount);
	void DisplayObjectiveMessage(const FString& Message);
	void DisplayAnnouncementMessage(const FString& Message);
	void DisplayTotalZombieKills(int32 TotalZombieKills);
	void DisplayZombieKills(int32 ZombieKills);
	void DisplayAmmo(int32 CurrentAmmo, int32 TotalAmmo);
	void DisplayWeaponType(FString Type);
	void DisplayCurrentHealth(int32 CurrentHealth);
	
	void SetCutsceneMode(bool bEnable);
protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	FPlayerData PlayerData;
private:
	UPROPERTY()
	class UOB_Widget* OB_Widget;
};
