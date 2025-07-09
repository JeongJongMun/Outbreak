// Fill out your copyright notice in the Description page of Project Settings.

#include "OBWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Outbreak/Game/OutBreakGameState.h"

void UOBWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld() || GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return; 
	}
	if (RenderTarget && MiniMapMaterial && MiniMapImage)
	{
		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(MiniMapMaterial, this);
		DynMat->SetTextureParameterValue("MinimapTexture", RenderTarget);
		MiniMapImage->SetBrushFromMaterial(DynMat);
	}
}

void UOBWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (const AOutBreakGameState* GS = GetWorld()->GetGameState<AOutBreakGameState>())
	{
		SetMatchTimeText(GS->GetMatchTime());
		SetCurrentPhaseText(GS->GetCurrentPhase());
	}
}
	
void UOBWidget::SetCutsceneMode(bool bEnable)
{
	ESlateVisibility NewVisibility = bEnable ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	// 컷씬 재생시 안보일 HUD & UI
	TArray<UWidget*> WidgetsToToggle = {
		MiniMapImage,
		AimImage,
		MatchTimeTextBlock,
		PhaseTextBlock,
		AlivePlayerCountTextBlock,
		AnnouncementTextBlock,
		TotalZombieKillsTextBlock,
		ZombieKillsTextBlock,
		AmmoTextBlock,
		WeaponTypeTextBlock,
		HealthBar,
		CurrentHealthTextBlock
	};
	for (UWidget* Widget : WidgetsToToggle)
	{
		if (Widget)
		{
			Widget->SetVisibility(NewVisibility);
		}
	}
}



void UOBWidget::SetMatchTimeText(float Time)
{
	if (MatchTimeTextBlock)
	{
		int32 TotalSeconds = FMath::FloorToInt(Time);
		int32 Minutes = TotalSeconds / 60;
		int32 Seconds = TotalSeconds % 60;
		FString TimeStr = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		MatchTimeTextBlock->SetText(FText::FromString(TimeStr));
	}
	
}

void UOBWidget::SetCurrentPhaseText(FString Phase)
{
	if (PhaseTextBlock)
	{
		FString PhaseStr = FString::Printf(TEXT("%s"), *Phase);
		PhaseTextBlock->SetText(FText::FromString(PhaseStr));
	}
}

void UOBWidget::SetAlivePlayerCountText(int32 Count)
{
	if (AlivePlayerCountTextBlock)
	{
		FString CountText = FString::Printf(TEXT("Alive Player : %d"), Count);
		AlivePlayerCountTextBlock->SetText(FText::FromString(CountText));
	}
}

void UOBWidget::SetAnnouncementText(FString AnnouncementText)
{
	if (AnnouncementTextBlock)
	{
		FString AnnouncementMsg = FString::Printf(TEXT("%s"), *AnnouncementText);
		AnnouncementTextBlock->SetText(FText::FromString(AnnouncementMsg));
	}
}

void UOBWidget::SetTotalZombieKillsText(int32 TotalKills)
{
	if (TotalZombieKillsTextBlock)
	{
		FString CountText = FString::Printf(TEXT("Total Kills : %d"), TotalKills);
		TotalZombieKillsTextBlock->SetText(FText::FromString(CountText));
	}
}

void UOBWidget::SetZombieKillsText(int32 Kills)
{
	if (ZombieKillsTextBlock)
	{
		FString CountText = FString::Printf(TEXT("Kills : %d"), Kills);
		ZombieKillsTextBlock->SetText(FText::FromString(CountText));
	}
}

void UOBWidget::SetAmmoText(int32 CurrentAmmo, int32 TotalAmmo)
{
	if (AmmoTextBlock)
	{
		FString AmmoStr = FString::Printf(TEXT("%d / %d"), CurrentAmmo, TotalAmmo);
		AmmoTextBlock->SetText(FText::FromString(AmmoStr));
	}
}

void UOBWidget::SetWeaponTypeText(FString Type)
{
	if (WeaponTypeTextBlock)
	{
		FString TypeStr = FString::Printf(TEXT("%s"), *Type);
		WeaponTypeTextBlock->SetText(FText::FromString(TypeStr));
	}
}

void UOBWidget::SetCurrentHealth(int32 CurrentHealth, float HealthPercent)
{
	UE_LOG(LogTemp, Warning, TEXT("SetCurrentHealth: Health=%d, Percent=%.2f"), CurrentHealth, HealthPercent);

	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}	
	if (CurrentHealthTextBlock)
	{
		FString HealthStr = FString::Printf(TEXT("%d / 100"), CurrentHealth);
		CurrentHealthTextBlock->SetText(FText::FromString(HealthStr));
	}
}



