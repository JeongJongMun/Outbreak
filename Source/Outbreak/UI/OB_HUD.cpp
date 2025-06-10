// Fill out your copyright notice in the Description page of Project Settings.


#include "OB_HUD.h"
#include "OB_Widget.h"
#include "Blueprint/UserWidget.h"

void AOB_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (UClass* WidgetClass = LoadClass<UOB_Widget>(nullptr, TEXT("/Game/UI/WBP_OBWidget.WBP_OBWidget_C")))
		OB_Widget = CreateWidget<UOB_Widget>(GetWorld(), WidgetClass);
		if (OB_Widget)
		{
			OB_Widget->AddToViewport();
			OB_Widget->SetVisibility(ESlateVisibility::Visible);
			OB_Widget->SetObjectiveText(""); // 목표 메시지만 비활성화
			OB_Widget->SetAnnouncementText(""); 
		}
}

void AOB_HUD::SetCutsceneMode(bool bEnable)
{
	if (OB_Widget)
	{
		OB_Widget->SetCutsceneMode(bEnable);
	}
}

void AOB_HUD::DisplayAlivePlayerCount(int32 AlivePlayerCount)
{
	if (OB_Widget)
	{
		OB_Widget->SetAlivePlayerCountText(AlivePlayerCount);
		OB_Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AOB_HUD::DisplayObjectiveMessage(const FString& Message)
{
	if (OB_Widget)
	{
		OB_Widget->SetObjectiveText(Message);
		OB_Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AOB_HUD::DisplayAnnouncementMessage(const FString& Message)
{
	if (OB_Widget)
	{
		OB_Widget->SetAnnouncementText(Message);
		OB_Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AOB_HUD::DisplayTotalZombieKills(int32 TotalKills)
{
	if (OB_Widget)
	{
		OB_Widget->SetTotalZombieKillsText(TotalKills);
		OB_Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AOB_HUD::DisplayZombieKills(int32 Kills)
{
	if (OB_Widget)
	{
		OB_Widget->SetZombieKillsText(Kills);
		OB_Widget->SetVisibility(ESlateVisibility::Visible);
	}
}
void AOB_HUD::DisplayAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
	if (OB_Widget)
	{
		OB_Widget->SetAmmoText(CurrentAmmo, TotalAmmo);
	}
}

void AOB_HUD::DisplayWeaponType(FString Type)
{
	if (OB_Widget)
	{
		OB_Widget->SetWeaponTypeText(Type);
	}
}
