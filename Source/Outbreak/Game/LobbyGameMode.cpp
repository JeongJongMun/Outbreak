#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	ConnectedPlayers = 0;
	UE_LOG(LogTemp, Warning , TEXT("대기방 시작"));
}

// 대기방 접속
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ConnectedPlayers++; // 대기방 접속 시 인원 수 증가

	UE_LOG(LogTemp, Warning, TEXT("플레이어 접속 : 현재 %d명"), ConnectedPlayers);
	StartMatchIfReady(); // 4명 접속 및 캐릭터 선택 완료 시 게임 시작
}

// 4명이상 대기방 접속 시 접속 거부
void ALobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
							   FString& ErrorMessage)
{
	if (ConnectedPlayers >= 4)
	{
		// 접속 거부 메시지 전달
		ErrorMessage = TEXT("대기방 인원이 가득 찼습니다. 접속이 거부되었습니다.");
		UE_LOG(LogTemp, Warning, TEXT("접속 거부: 최대 인원 초과"));
		return;
	}

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

// 대기방 4명 조건 확인 및 4명일 시 시작
void ALobbyGameMode::StartMatchIfReady()
{
	// TODO : 대기방에 접속한 인원이 4명일 뿐아니라 4명 모두 캐릭터 선택 완료시에 게임 시작하도록 조건 추가
	if (!HasMatchStarted() && ConnectedPlayers == 4)
	{
		if (HasAuthority())
		{
			StartMatch(); // MatchState = InProgress
			UE_LOG(LogTemp, Warning, TEXT("매치 시작 조건 만족"));
			GetWorld()->ServerTravel("/Game/Maps/FirstPhase?listen", true);
		}
	}
}