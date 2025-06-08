// Fill out your copyright notice in the Description page of Project Settings.


#include "SafeZoneController.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "CutsceneManager.h"
#include "InGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


ASafeZoneController::ASafeZoneController()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StartSafeZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("StartSafeZone"));
	StartSafeZoneCollision->SetupAttachment(RootComponent);

	EndSafeZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EndSafeZone"));
	EndSafeZoneCollision->SetupAttachment(RootComponent);
}

void ASafeZoneController::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어가 콜리전에 들어오거나 나갈때 실행할 함수 지정
	if (StartSafeZoneCollision)
	{
		StartSafeZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ASafeZoneController::OnStartZoneEnter);
		UE_LOG(LogTemp, Warning, TEXT("[SafeZone] 시작 존에 진입해서 등록"));

		TArray<AActor*> OverlappingActors;
		StartSafeZoneCollision->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());
		for (AActor* Actor : OverlappingActors)
		{
			if (ACharacter* Character = Cast<ACharacter>(Actor))
			{
				PlayersInStartZone.Add(Character);
			}
		}
		
		StartSafeZoneCollision->OnComponentEndOverlap.AddDynamic(this, &ASafeZoneController::OnStartZoneExit);
	}
	if (EndSafeZoneCollision)
	{
		EndSafeZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ASafeZoneController::OnEndZoneEnter);

	}
	InGameModeRef = Cast<AInGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	CutsceneManager = NewObject<UCutsceneManager>();
	CutsceneManager->AddToRoot();
	CutsceneManager->Init(GetWorld());
}

void ASafeZoneController::OnEndZoneEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("[SafeZone] 캐릭터 %s 종료 존에 진입"), *Character->GetName());

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

void ASafeZoneController::OnStartZoneEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character && !PlayersInStartZone.Contains(Character))
	{
		PlayersInStartZone.Add(Character);
		UE_LOG(LogTemp, Warning, TEXT("[SafeZone] %s 시작 존에 진입해서 등록"), *Character->GetName());
	}
}


void ASafeZoneController::OnStartZoneExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("[SafeZone] 캐릭터 %s 시작 존에서 이탈"), *Character->GetName());

		PlayersInStartZone.Remove(Character);
		if (PlayersInStartZone.Num() == 0)
		{
			// TODO : 좀비 AI 및 스폰 활성화 코드 작성
			// 단, 컷씬 추가시 활성화 시간을 컷씬 종료시에 해야함
			if (CutsceneManager && !CutsceneManager->bHasPlayedCutscene)
			{
				FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
				FString ObjectiveMessage;
				if (CurrentLevel == TEXT("FirstPhase")) ObjectiveMessage = TEXT("목표 : 숲을 탈출하라 !!");
				else if (CurrentLevel == TEXT("SecondPhase")) ObjectiveMessage = TEXT("목표 : 마을을 탈출하라 !!");
				else if (CurrentLevel == TEXT("ThirdPhase")) ObjectiveMessage = TEXT("목표 : 건물 10층에 도달하라 !!");
				else ObjectiveMessage = TEXT("목표 : 보스를 처치하고 탈출하라 !");

				CutsceneManager->PlayCutscene(CutsceneSequence, ObjectiveMessage);
				CutsceneManager->bHasPlayedCutscene = true;
				StartSafeZoneCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}
