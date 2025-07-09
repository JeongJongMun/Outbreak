// Fill out your copyright notice in the Description page of Project Settings.


#include "OutBreakPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Outbreak/UI/OBHUD.h"

AOutBreakPlayerState::AOutBreakPlayerState()
{
	// 오류 방지를 위해 초기화
	PlayerNickname = TEXT("Player0");
	CharacterClass = TEXT("Default");
	ZombieKills = 0;
	TotalDamageDealt = 0.f;
	DownCount = 0;
	AssistCount = 0;
	CharacterStatus = ECharacterStatus::Alive;
	CurrentExp = 0;
	CharacterLevel = 1;
}
void AOutBreakPlayerState::OnRep_PlayerNickname()
{
	UE_LOG(LogTemp, Log, TEXT("플레이어 닉네임: %s"), *PlayerNickname);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}


void AOutBreakPlayerState::OnRep_CharacterClass()
{
	UE_LOG(LogTemp, Log, TEXT("캐릭터 종류 변경: %s"), *CharacterClass);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakPlayerState::OnRep_ZombieKills()
{
	UE_LOG(LogTemp, Log, TEXT("좀비 처치 수 변경: %d"), ZombieKills);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PC->IsLocalController() && PC->PlayerState == this)
		{
			if (AOBHUD* HUD = Cast<AOBHUD>(PC->GetHUD()))
			{
				HUD->DisplayZombieKills(ZombieKills);
			}
		}
	}
}

void AOutBreakPlayerState::AddZombieKill()
{
	ZombieKills++;
}

void AOutBreakPlayerState::OnRep_TotalDamageDealt()
{
	UE_LOG(LogTemp, Log, TEXT("누적 데미지 변경: %.1f"), TotalDamageDealt);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakPlayerState::OnRep_DownCount()
{
	UE_LOG(LogTemp, Log, TEXT("기절 수 변경: %d"), DownCount);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakPlayerState::OnRep_AssistCount()
{
	UE_LOG(LogTemp, Log, TEXT("어시스트 수 변경: %d"), AssistCount);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakPlayerState::OnRep_CharacterStatus()
{
	UE_LOG(LogTemp, Log, TEXT("상태 변경: %s"), *UEnum::GetValueAsString(CharacterStatus));
	// TODO: 생존/기절/사망에 따른 UI 아이콘 처리
}

void AOutBreakPlayerState::OnRep_CurrentExp()
{
	UE_LOG(LogTemp, Log, TEXT("경험치 변경: %d"), CurrentExp);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

void AOutBreakPlayerState::OnRep_CharacterLevel()
{
	UE_LOG(LogTemp, Log, TEXT("레벨 변경: %d"), CharacterLevel);
	// TODO: HUD 또는 UI 업데이트 함수 호출
}

// 서버에서 값이 변경되면 자동으로 클라이언트에 값을 받게 하는 함수
void AOutBreakPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOutBreakPlayerState, PlayerNickname);
	DOREPLIFETIME(AOutBreakPlayerState, CharacterClass);
	DOREPLIFETIME(AOutBreakPlayerState, ZombieKills);
	DOREPLIFETIME(AOutBreakPlayerState, TotalDamageDealt);
	DOREPLIFETIME(AOutBreakPlayerState, DownCount);
	DOREPLIFETIME(AOutBreakPlayerState, AssistCount);
	DOREPLIFETIME(AOutBreakPlayerState, CharacterStatus);
	DOREPLIFETIME(AOutBreakPlayerState, CurrentExp);
	DOREPLIFETIME(AOutBreakPlayerState, CharacterLevel);
}
