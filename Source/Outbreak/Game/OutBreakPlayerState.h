// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OutBreakPlayerState.generated.h"

UENUM(BlueprintType)
enum class ECharacterStatus : uint8
{
	Alive,
	Downed,
	Dead
};

UCLASS()
class OUTBREAK_API AOutBreakPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AOutBreakPlayerState();

	// 캐릭터 종류
	UPROPERTY(ReplicatedUsing = OnRep_CharacterClass, BlueprintReadOnly, Category = "Player Info")
	FString CharacterClass;

	// 좀비 킬 수(개인)
	UPROPERTY(ReplicatedUsing = OnRep_ZombieKills, BlueprintReadOnly, Category = "Combat")
	int32 ZombieKills;

	// 총 데미지량(개인)
	UPROPERTY(ReplicatedUsing = OnRep_TotalDamageDealt, BlueprintReadOnly, Category = "Combat")
	float TotalDamageDealt;

	// 기절 수
	UPROPERTY(ReplicatedUsing = OnRep_DownCount, BlueprintReadOnly, Category = "Combat")
	int32 DownCount;

	// 어시스트 수(팀원 부활)
	UPROPERTY(ReplicatedUsing = OnRep_AssistCount, BlueprintReadOnly, Category = "Combat")
	int32 AssistCount;

	// 캐릭터 상태(생존, 기절, 죽음)
	UPROPERTY(ReplicatedUsing = OnRep_CharacterStatus, BlueprintReadOnly, Category = "Status")
	ECharacterStatus CharacterStatus;

	// 현재 경험치
	UPROPERTY(ReplicatedUsing = OnRep_CurrentExp, BlueprintReadOnly, Category = "Status")
	int32 CurrentExp;

	// 캐릭터 현재 레벨
	UPROPERTY(ReplicatedUsing = OnRep_CharacterLevel, BlueprintReadOnly, Category = "Status")
	int32 CharacterLevel;

protected:
	UFUNCTION()
	void OnRep_CharacterClass();

	UFUNCTION()
	void OnRep_ZombieKills();

	UFUNCTION()
	void OnRep_TotalDamageDealt();

	UFUNCTION()
	void OnRep_DownCount();

	UFUNCTION()
	void OnRep_AssistCount();

	UFUNCTION()
	void OnRep_CharacterStatus();

	UFUNCTION()
	void OnRep_CurrentExp();

	UFUNCTION()
	void OnRep_CharacterLevel();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	


};
