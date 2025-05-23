// Fill out your copyright notice in the Description page of Project Settings.


#include "SafeZoneController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "InGameMode.h"

ASafeZoneController::ASafeZoneController()
{
	PrimaryActorTick.bCanEverTick = false;

	// 시작 지점 Collision 컴포넌트 생성 및 설정
	StartSafeZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("StartSafeZone"));
	RootComponent = StartSafeZoneCollision;
	StartSafeZoneCollision->InitBoxExtent(FVector(500.f, 500.f, 300.f)); // 가로, 세로, 높이 설정
	StartSafeZoneCollision->SetCollisionProfileName(TEXT("Trigger"));
	StartSafeZoneCollision->SetGenerateOverlapEvents(true);

	// 종료 지점 Collision 컴포넌트 생성 및 설정
	EndSafeZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EndSafeZone"));
	RootComponent = EndSafeZoneCollision;
	EndSafeZoneCollision->InitBoxExtent(FVector(500.f, 500.f, 300.f)); // 가로, 세로, 높이 설정
	EndSafeZoneCollision->SetCollisionProfileName(TEXT("Trigger"));
	EndSafeZoneCollision->SetGenerateOverlapEvents(true);
}

void ASafeZoneController::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어가 콜리전에 들어오거나 나갈때 실행할 함수 지정
	StartSafeZoneCollision->OnComponentEndOverlap.AddDynamic(this, &ASafeZoneController::OnStartZoneExit);
	EndSafeZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ASafeZoneController::OnEndZoneEnter);

	InGameModeRef = Cast<AInGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ASafeZoneController::OnEndZoneEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		PlayersInEndZone.Add(Character); // 들어온 캐릭터 목록에 추가

		int32 TotalPlayers = UGameplayStatics::GetNumPlayerControllers(GetWorld());

		if (PlayersInEndZone.Num() == TotalPlayers)
		{
			// TODO : 좀비 AI 및 스폰 비활성화 코드 작성
			// 컷씬 추가시 즉시 비활성화
			if (InGameModeRef && InGameModeRef->IsMatchInProgress())
			{
				InGameModeRef->EndMatch();
				InGameModeRef->ProceedToNextLevel();
			}
		}
	}
}

void ASafeZoneController::OnStartZoneExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		PlayersInStartZone.Remove(Character);
		if (PlayersInStartZone.Num() == 0)
		{
			// TODO : 좀비 AI 및 스폰 활성화 코드 작성
			// 단, 컷씬 추가시 활성화 시간을 컷씬 종료시에 해야함
			if (InGameModeRef && !InGameModeRef->HasMatchStarted())
			{
				InGameModeRef->StartMatch();
			}
		}
	}
}
