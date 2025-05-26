// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMode.h"

#include "OutBreakGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Set.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/UI/OB_HUD.h"


AInGameMode::AInGameMode()
{
	DefaultPawnClass = ACharacterPlayer::StaticClass();
	HUDClass = AOB_HUD::StaticClass();
	GameStateClass = AOutBreakGameState::StaticClass();
}
void AInGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartMatch();
	UE_LOG(LogTemp, Warning, TEXT("게임 시작됨"));
}

// 레벨 전환 함수
void AInGameMode::ProceedToNextLevel() const
{
	if (!HasAuthority()) return;

	FName CurrentLevel = *UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	if (CurrentLevel == TEXT("FirstPhase"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("SecondPhase"));
	}
	else if (CurrentLevel == TEXT("SecondPhase"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("ThirdPhase"));
	}
	else if (CurrentLevel == TEXT("ThirdPhase"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("LastPhase"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("게임 종료"));
		// TODO: 대기방 레벨로 이동 코드 작성
		// 단, 마지막 페이즈는 보스 처치시 게임이 완료 됨(SafeZoneCollision이 없음)
	}
}