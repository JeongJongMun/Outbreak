// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Define.generated.h"

#define CURRENT_CONTEXT *FString(__FUNCTION__)

UENUM()
enum class EZombieStateType : uint8
{
	None	UMETA(DisplayName = "None"),
	Idle	UMETA(DisplayName = "휴면"),
	Wander	UMETA(DisplayName = "배회"),
	Alert	UMETA(DisplayName = "경계"),
	Chase	UMETA(DisplayName = "추적"),
	Attack	UMETA(DisplayName = "공격"),
	Stun	UMETA(DisplayName = "기절"),
	Die 	UMETA(DisplayName = "사망"),
};

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	FPS,
	TopView
};

UENUM()
enum class EPlayerControlType : uint8
{
	Shoulder,
	Top,
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None,
	Player UMETA(DisplayName = "플레이어"),
	Zombie UMETA(DisplayName = "좀비"),
};

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	// TODO : 캐릭터 4가지 기획 이름으로 변경
	None,
	Player1 UMETA(DisplayName = "플레이어1"),
	Player2 UMETA(DisplayName = "플레이어2"),
	Player3 UMETA(DisplayName = "플레이어3"),
	Player4 UMETA(DisplayName = "플레이어4"),
};

UENUM(BlueprintType)
enum class EZombieType : uint8
{
	None,
	Normal      UMETA(DisplayName = "일반 좀비"),
	Special     UMETA(DisplayName = "특수 좀비"),
	Boss        UMETA(DisplayName = "보스 좀비"),
};

UENUM(BlueprintType)
enum class EZombieSubType : uint8
{
	None,
	// 일반 좀비 서브타입
	Walker      UMETA(DisplayName = "걷는 좀비"),
	Runner      UMETA(DisplayName = "뛰는 좀비"),
	Fat         UMETA(DisplayName = "뚱뚱 좀비"),
	Soldier     UMETA(DisplayName = "군인 좀비"),
    
	// 특수 좀비 서브타입
	GymRat      UMETA(DisplayName = "헬창 좀비"),
	Radioactive UMETA(DisplayName = "방사능 좀비"),
	Ghost       UMETA(DisplayName = "유체화 좀비"),
	Shield      UMETA(DisplayName = "방패 좀비"),
	Berserker   UMETA(DisplayName = "광분화 좀비"),
    
	// 보스 좀비는 별도 관리
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	ECharacterType CharacterType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	uint8 bIsCanRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int RegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxExtraHealth;
};

USTRUCT(BlueprintType)
struct FPlayerData : public FCharacterData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EPlayerType PlayerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int BottomHealth;
};

USTRUCT(BlueprintType)
struct FZombieData : public FCharacterData
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EZombieType ZombieType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EZombieSubType ZombieSubType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float TurnRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AggroRange;

};

class OUTBREAK_API Define { };