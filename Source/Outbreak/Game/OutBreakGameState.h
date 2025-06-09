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
	
	// 서버가 가진 데이터를 클라이언트와 자동으로 동기화 하기 위해 ReplicatedUsing 사용 -> Server RPC 방식
	// 게임 진행 정보
	UPROPERTY(Replicated, BlueprintReadOnly, Category ="Game Progress")
	float MatchTime; // 게임 진행 시간

	UPROPERTY(Replicated, BlueprintReadOnly, Category ="Game Progress")
	FString CurrentPhase; // 현재 페이즈

	// 전체 통계 정보
	UPROPERTY(ReplicatedUsing = OnRep_TotalZombieKills, BlueprintReadOnly, Category ="Statistics")
	int32 TotalZombieKills; // 전체 좀비 킬 수

	UPROPERTY(ReplicatedUsing = OnRep_AlivePlayerCount, BlueprintReadOnly, Category ="Statistics")
	int32 AlivePlayerCount; // 살아있는 플레이어 수

	UPROPERTY(ReplicatedUsing = OnRep_DeadPlayerCount, BlueprintReadOnly, Category ="Statistics")
	int32 DeadPlayerCount; // 죽은 플레이어 수

	UPROPERTY(ReplicatedUsing = OnRep_DownedPlayerCount, BlueprintReadOnly, Category ="Statistics")
	int32 DownedPlayerCount; // 기절 상태 플레이어 수

	// 글로벌 UI 메시지
	UPROPERTY(ReplicatedUsing = OnRep_AnnouncementMessage, BlueprintReadOnly, Category="UI")
	FString AnnouncementMessage; // 알림 UI

	UPROPERTY(ReplicatedUsing = OnRep_ObjectMessage, BlueprintReadOnly, Category="UI")
	FString ObjectiveMessage; // 목표 UI

	UPROPERTY(ReplicatedUsing = OnRep_EventAlertMessage, BlueprintReadOnly, Category="UI")
	FString EventAlertMessage; // 경고 UI

	FORCEINLINE float GetMatchTime() const { return MatchTime; }
	FORCEINLINE FString GetCurrentPhase() const { return CurrentPhase; }
	
	UFUNCTION(BlueprintCallable)
	void SetObjectiveMessage(const FString& NewMessage);

	TObjectPtr<ACharacterSpawnManager> GetSpawnManager() const { return SpawnManager; }

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

private:
	UPROPERTY()
	ACharacterSpawnManager* SpawnManager;
};
