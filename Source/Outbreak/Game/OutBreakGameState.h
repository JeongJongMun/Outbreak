#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Outbreak/Character/Zombie/CharacterSpawnManager.h"
#include "OutBreakGameState.generated.h"

UCLASS()
class OUTBREAK_API AOutBreakGameState : public AGameState
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
public:
	AOutBreakGameState();
	
// --------------------
// Getters
// --------------------
	
	FORCEINLINE float GetMatchTime() const { return MatchTime; }
	FORCEINLINE FString GetCurrentPhase() const { return CurrentPhase; }

	TObjectPtr<ACharacterSpawnManager> GetSpawnManager() const { return SpawnManager; }
	
// --------------------
// Variables
// --------------------
	
public:
	// --------Spawner--------
	UPROPERTY()
	AActor* SpawnerInstance;
	
	// --------GameState Info--------
	UPROPERTY(Replicated)
	float MatchTime; // 게임 진행 시간

	UPROPERTY(Replicated)
	FString CurrentPhase; // 현재 페이즈
	
	UPROPERTY(ReplicatedUsing = OnRep_TotalZombieKills)
	int32 TotalZombieKills; // 전체 좀비 킬 수

	UPROPERTY(ReplicatedUsing = OnRep_AlivePlayerCount)
	int32 AlivePlayerCount; // 살아있는 플레이어 수

	UPROPERTY(ReplicatedUsing = OnRep_DeadPlayerCount)
	int32 DeadPlayerCount; // 죽은 플레이어 수

	UPROPERTY(ReplicatedUsing = OnRep_DownedPlayerCount)
	int32 DownedPlayerCount; // 기절 상태 플레이어 수
	
	UPROPERTY(ReplicatedUsing = OnRep_AnnouncementMessage)
	FString AnnouncementMessage; // 알림 UI

	UPROPERTY(ReplicatedUsing = OnRep_ObjectMessage)
	FString ObjectiveMessage; // 목표 UI

	UPROPERTY(ReplicatedUsing = OnRep_EventAlertMessage)
	FString EventAlertMessage; // 경고 UI
	
private:
	UPROPERTY()
	ACharacterSpawnManager* SpawnManager;
	
// --------------------
// Functions
// --------------------

public:
	UFUNCTION()
	void SetObjectiveMessage(const FString& NewMessage);
	
	UFUNCTION()
	void AddTotalZombieKill();

	UFUNCTION()
	void SpawnerSetup();
	
protected:
	UFUNCTION()
	void OnRep_TotalZombieKills();

	UFUNCTION()
	void OnRep_AlivePlayerCount();

	UFUNCTION()
	void OnRep_DeadPlayerCount();

	UFUNCTION()
	void OnRep_DownedPlayerCount();
	
	UFUNCTION()
	void OnRep_AnnouncementMessage();

	UFUNCTION()
	void OnRep_ObjectMessage();

	UFUNCTION()
	void OnRep_EventAlertMessage();
};
