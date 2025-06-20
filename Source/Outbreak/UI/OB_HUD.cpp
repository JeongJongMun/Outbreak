// Fill out your copyright notice in the Description page of Project Settings.


#include "OB_HUD.h"
#include "OB_Widget.h"
#include "Blueprint/UserWidget.h"

AOB_HUD::AOB_HUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBPClass(TEXT("/Game/UI/WBP_OBWidget"));
	if (WidgetBPClass.Succeeded())
	{
		WidgetClass = WidgetBPClass.Class;
	}
}



void AOB_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
		OB_Widget = CreateWidget<UOB_Widget>(GetWorld(), WidgetClass);
		if (OB_Widget)
		{
			OB_Widget->AddToViewport();
			OB_Widget->SetVisibility(ESlateVisibility::Visible);
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

void AOB_HUD::DisplayCurrentHealth(int32 CurrentHealth)
{
	if (OB_Widget)
	{
		// TODO : PlayerData 에서 MaxHealth 가져오기
		float Percent = (float)CurrentHealth / 100.0f;
		OB_Widget->SetCurrentHealth(CurrentHealth, Percent);
	}
}