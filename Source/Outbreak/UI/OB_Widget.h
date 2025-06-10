// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OB_Widget.generated.h"

class UTextBlock;

UCLASS()
class OUTBREAK_API UOB_Widget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	
	void SetObjectiveText(const FString& Text);
	void SetMatchTimeText(float Time);
	void SetCurrentPhaseText(FString Phase);
	void SetAlivePlayerCountText(int32 Count);
	void SetAnnouncementText(FString AnnouncementText);
	void SetTotalZombieKillsText(int32 TotalKills);
	void SetZombieKillsText(int32 Kills);
	
	void SetCutsceneMode(bool bEnable);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* MiniMapImage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchTimeTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PhaseTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AlivePlayerCountTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ObjectiveTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnouncementTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalZombieKillsTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ZombieKillsTextBlock;
};
