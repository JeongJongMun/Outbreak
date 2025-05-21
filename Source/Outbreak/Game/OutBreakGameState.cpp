// Fill out your copyright notice in the Description page of Project Settings.


#include "OutBreakGameState.h"
#include "Net/UnrealNetwork.h"

AOutBreakGameState::AOutBreakGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	// 오류 방지를 위해 값 초기화
	MatchTime = 0.f;
	CurrentPhase = 0;
	TotalZombieKills = 0;
	AlivePlayerCount = 0;
	DeadPlayerCount = 0;
	DownedPlayerCount = 0;
	AnnouncementMessage = TEXT("");
	ObjectiveMessage = TEXT("");
	EventAlertMessage = TEXT("");
}

void AOutBreakGameState::OnRep_MatchTime()
{
	UE_LOG(LogTemp, Log, TEXT("게임 시간 변경: %f"), MatchTime);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_CurrentPhase()
{
	UE_LOG(LogTemp, Log, TEXT("현재 페이즈 변경: %d"), CurrentPhase);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_TotalZombieKills()
{
	UE_LOG(LogTemp, Log, TEXT("총 좀비 처치 수 변경: %d"), TotalZombieKills);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_AlivePlayerCount()
{
	UE_LOG(LogTemp, Log, TEXT("생존 플레이어 수 변경: %d"), AlivePlayerCount);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_DeadPlayerCount()
{
	UE_LOG(LogTemp, Log, TEXT("죽은 플레이어 수 변경: %d"), AlivePlayerCount);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_DownedPlayerCount()
{
	UE_LOG(LogTemp, Log, TEXT("기절 플레이어 수 변경: %d"), DownedPlayerCount);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_AnnouncementMessage()
{
	UE_LOG(LogTemp, Log, TEXT("알림 : %s"), *AnnouncementMessage);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_ObjectMessage()
{
	UE_LOG(LogTemp, Log, TEXT("게임 목표 : %s"), *ObjectiveMessage);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakGameState::OnRep_EventAlertMessage()
{
	UE_LOG(LogTemp, Log, TEXT("경고! %s"), *EventAlertMessage);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

// 서버에서 값이 변경되면 자동으로 클라이언트에 값을 받게 하는 함수
void AOutBreakGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOutBreakGameState, MatchTime);
	DOREPLIFETIME(AOutBreakGameState, CurrentPhase);
	DOREPLIFETIME(AOutBreakGameState, TotalZombieKills);
	DOREPLIFETIME(AOutBreakGameState, AlivePlayerCount);
	DOREPLIFETIME(AOutBreakGameState, DownedPlayerCount);
	DOREPLIFETIME(AOutBreakGameState, AnnouncementMessage);
	DOREPLIFETIME(AOutBreakGameState, ObjectiveMessage);
	DOREPLIFETIME(AOutBreakGameState, EventAlertMessage);
}




