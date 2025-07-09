// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"


UCLASS()
class OUTBREAK_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;
	ALobbyGameMode();

	// 대기방 상태 관리 -> 인원 수
    UPROPERTY(BlueprintReadOnly)
	int32 ConnectedPlayers;

	// 접속한 플레이어 수 확인
	void PostLogin(APlayerController* NewPlayer) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	// 4명 모두 접속 후 캐릭터 선택 완료 시 게임 시작 처리
	void StartMatchIfReady();	
};
