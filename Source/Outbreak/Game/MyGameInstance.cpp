// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();

	// 초기 설정
	PlayerNickname = TEXT("Write Here"); // 플레이어 닉네임 설정(추후 수정)
	GraphicsQuality = 2; // 중간 품질(추후 수정) 1 / 2 / 3 -> 하 중 상 옵션
	MasterVolume = 0.8f; // 임시 설정(추후 수정)
	bIsInQueue = false; // 대기 상태
	bIsLoggedIn = true; // 로그인 여부 확인
	CurrentSessionID = TEXT("Write Here"); // 세션 관리(추후 수정)
	UserID = TEXT("Write Here"); // 유저 ID(추후 수정)

	UE_LOG(LogTemp, Warning, TEXT("GameInstance 초기화 완료"));
	
}
