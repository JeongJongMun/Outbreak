// Fill out your copyright notice in the Description page of Project Settings.

#include "OB_Widget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Outbreak/Game/OutBreakGameState.h"

void UOB_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	UTextureRenderTarget2D* RenderTarget = LoadObject<UTextureRenderTarget2D>(nullptr, TEXT("/Game/UI/MiniMap/RT_Minimap.RT_Minimap"));
	if (RenderTarget && MiniMapImage)
	{
		UMaterialInterface* MinimapMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/MiniMap/MiniMapMaterial.MiniMapMaterial"));
		if (MinimapMaterial)
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(MinimapMaterial, this);
			DynMat->SetTextureParameterValue("MinimapTexture", Cast<UTexture>(RenderTarget));
			MiniMapImage->SetBrushFromMaterial(DynMat);
		}
	}
}

void UOB_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (const AOutBreakGameState* GS = GetWorld()->GetGameState<AOutBreakGameState>())
	{
		SetMatchTimeText(GS->GetMatchTime());
		SetCurrentPhaseText(GS->GetCurrentPhase());
	}
}
	
void UOB_Widget::SetCutsceneMode(bool bEnable)
{
	ESlateVisibility NewVisibility = bEnable ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	if (MiniMapImage) MiniMapImage->SetVisibility(NewVisibility);
	if (MatchTimeTextBlock) MatchTimeTextBlock->SetVisibility(NewVisibility);
	if (PhaseTextBlock) PhaseTextBlock->SetVisibility(NewVisibility);
	if (AlivePlayerCountTextBlock) AlivePlayerCountTextBlock->SetVisibility(NewVisibility);
	if (AnnouncementTextBlock) AnnouncementTextBlock->SetVisibility(NewVisibility);
}



void UOB_Widget::SetMatchTimeText(float Time)
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

void UOB_Widget::SetCurrentPhaseText(FString Phase)
{
	if (PhaseTextBlock)
	{
		FString PhaseStr = FString::Printf(TEXT("%s"), *Phase);
		PhaseTextBlock->SetText(FText::FromString(PhaseStr));
	}
}

void UOB_Widget::SetAlivePlayerCountText(int32 Count)
{
	if (AlivePlayerCountTextBlock)
	{
		FString CountText = FString::Printf(TEXT("Alive Player : %d"), Count);
		AlivePlayerCountTextBlock->SetText(FText::FromString(CountText));
	}
}

void UOB_Widget::SetObjectiveText(const FString& Text)
{
	if (ObjectiveTextBlock)
	{
		ObjectiveTextBlock->SetText(FText::FromString(Text));
	}
}

void UOB_Widget::SetAnnouncementText(FString AnnouncementText)
{
	if (AnnouncementTextBlock)
	{
		FString AnnouncementMsg = FString::Printf(TEXT("%s"), *AnnouncementText);
		AnnouncementTextBlock->SetText(FText::FromString(AnnouncementMsg));
	}
}




