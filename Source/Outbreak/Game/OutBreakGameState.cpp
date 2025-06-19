// Fill out your copyright notice in the Description page of Project Settings.


#include "OutBreakGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Outbreak/Character/Zombie/CharacterSpawnManager.h"
#include "Outbreak/UI/OB_HUD.h"

AOutBreakGameState::AOutBreakGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	// 오류 방지를 위해 값 초기화
	TotalZombieKills = 0;
	AlivePlayerCount = 0;
	DeadPlayerCount = 0;
	DownedPlayerCount = 0;
	AnnouncementMessage = TEXT("");
	ObjectiveMessage = TEXT("");
	EventAlertMessage = TEXT("");
}

void AOutBreakGameState::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (CurrentLevel == TEXT("FirstPhase")) CurrentPhase = "LEVEL 1 : Lush Forest";
	else if (CurrentLevel == TEXT("SecondPhase")) CurrentPhase = "LEVEL 2 : Devastated Village";
	else if (CurrentLevel == TEXT("ThirdPhase")) CurrentPhase = "LEVEL 3 : Skyscrapers";
	else if (CurrentLevel == TEXT("LastPhase")) CurrentPhase = "LEVEL 4 : Last Forest";
}

void AOutBreakGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		MatchTime += DeltaTime; // 게임 시간 업데이트

		// 생존 플레이어 수 표시
		int32 AliveCount = 0;
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				APawn* Pawn = PC->GetPawn();
				if (Pawn && !Pawn->IsPendingKillPending())
				{
					AliveCount++;
				}
			}
		}
		if (AlivePlayerCount != AliveCount)
		{
			AlivePlayerCount = AliveCount;
			OnRep_AlivePlayerCount();
		}

		// 모든 플레이어 사망 시 알림 메시지
		if (AlivePlayerCount == 0 && AnnouncementMessage.IsEmpty())
		{
			AnnouncementMessage = TEXT("All Players are dead!!");
			OnRep_AnnouncementMessage();
		} 
	}
}

void AOutBreakGameState::SpawnerSetup()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnManager = GetWorld()->SpawnActor<ACharacterSpawnManager>(ACharacterSpawnManager::StaticClass(), SpawnParams);

	if (HasAuthority())
	{
		UClass* SpawnerClass = StaticLoadClass(
			AActor::StaticClass(),
			nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/SSS/Blueprints/BP_SwarmSpawner.BP_SwarmSpawner_C'")
		);

		if (SpawnerClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Success!"));
			SpawnerInstance = GetWorld()->SpawnActor<AActor>(SpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
		}
	}
}


void AOutBreakGameState::OnRep_TotalZombieKills()
{
	UE_LOG(LogTemp, Log, TEXT("총 좀비 처치 수 변경: %d"), TotalZombieKills);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PC->IsLocalController())
		{
			if (AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD()))
			{
				HUD->DisplayTotalZombieKills(TotalZombieKills);
			}
		}
	}
}

void AOutBreakGameState::AddTotalZombieKill()
{
	TotalZombieKills++;
}


void AOutBreakGameState::OnRep_AlivePlayerCount()
{
	UE_LOG(LogTemp, Log, TEXT("생존 플레이어 수 변경: %d"), AlivePlayerCount);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PC->IsLocalController())
		{
			if (AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD()))
			{
				HUD->DisplayAlivePlayerCount(AlivePlayerCount);
			}	
		}
	}
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
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PC->IsLocalController())
		{
			if (AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD()))
			{
				HUD->DisplayAnnouncementMessage(ObjectiveMessage);
			}
		}
	}
}

void AOutBreakGameState::OnRep_ObjectMessage()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PC->IsLocalController())
		{
			if (AOB_HUD* HUD = Cast<AOB_HUD>(PC->GetHUD()))
			{
				HUD->DisplayObjectiveMessage(ObjectiveMessage);
			}
		}
	}
}
void AOutBreakGameState::SetObjectiveMessage(const FString& NewMessage)
{
	ObjectiveMessage = NewMessage;
	OnRep_ObjectMessage();
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




