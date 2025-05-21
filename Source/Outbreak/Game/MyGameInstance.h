// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class OUTBREAK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 플레이어 닉네임 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString PlayerNickname;

	// 게임 그래픽 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 GraphicsQuality;

	// 게임 오디오 설정 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float MasterVolume;

	// 매치 대기열 상태 관리(대기중 인지, 게임 시작 상태 인지)
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsInQueue;

	//게임 흐름 파악 및 세션 상태 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString CurrentSessionID;

	// 사용자 로그인 상태 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsLoggedIn;

	// 유저 고유 ID 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString UserID;

	// 선택한 캐릭터 종류 관리
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<ACharacter> SelectedCharacter;

	virtual void Init() override;
};
