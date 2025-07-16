#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Outbreak/Manager/CharacterSpawnManager.h"
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
	
private:
	UPROPERTY()
	ACharacterSpawnManager* SpawnManager;
	
// --------------------
// Functions
// --------------------

public:	
	UFUNCTION()
	void AddTotalZombieKill();

	UFUNCTION()
	void SpawnerSetup();
	
protected:
	UFUNCTION()
	void OnRep_TotalZombieKills();

	UFUNCTION()
	void OnRep_AlivePlayerCount();
	
};
