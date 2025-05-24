// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CutsceneManager.h"
#include "GameFramework/Actor.h"
#include "LevelSequence.h"
#include "SafeZoneController.generated.h"


class UBoxComponent;
class AInGameMode;

UCLASS()
class OUTBREAK_API ASafeZoneController : public AActor
{
	GENERATED_BODY()
	
public:	
	ASafeZoneController();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditInstanceOnly, Category = "SafeZone")
	UBoxComponent* StartSafeZoneCollision;

	UPROPERTY(EditInstanceOnly, Category = "SafeZone")
	UBoxComponent* EndSafeZoneCollision;

	// 플레이어가 콜리전 박스 안으로 들어왔을 때 호출
	UFUNCTION()
	void OnEndZoneEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	// 플레이어가 콜리전 박스 밖으로 나갔을 때 호출
	UFUNCTION()
	void OnStartZoneExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	TSet<AActor*> PlayersInStartZone;
	TSet<AActor*> PlayersInEndZone;

	UPROPERTY()
	AInGameMode* InGameModeRef;
	
	UPROPERTY()
	UCutsceneManager* CutsceneManager;
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	ULevelSequence* CutsceneSequence;
};
