// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Define.generated.h"

#define CURRENT_CONTEXT *FString(__FUNCTION__)

UENUM()
enum class ECharacterBodyType : uint8
{
	Normal,
	Muscle,
	Fat,
};

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
	// TODO : Change Real Player Name
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
struct FCharacterBaseData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float MaxRunSpeed;

	UPROPERTY(EditAnywhere)
	uint8 bIsCanRun;

	UPROPERTY(EditAnywhere)
	float AttackRate;

	UPROPERTY(EditAnywhere)
	int RegenRate;

	UPROPERTY(EditAnywhere)
	int MaxHealth;

	UPROPERTY(EditAnywhere)
	int MaxExtraHealth;
};

USTRUCT(BlueprintType)
struct FPlayerData : public FCharacterBaseData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EPlayerType PlayerType;

	UPROPERTY(EditAnywhere)
	int BottomHealth;

	UPROPERTY(EditAnywhere)
	float MaxCrouchSpeed;
};

USTRUCT(BlueprintType)
struct FZombieData : public FCharacterBaseData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EZombieType ZombieType;
	
	UPROPERTY(EditAnywhere)
	EZombieSubType ZombieSubType;
	
	UPROPERTY(EditAnywhere)
	float MaxWanderSpeed;
	
	UPROPERTY(EditAnywhere)
	float AttackDamage;
    
	UPROPERTY(EditAnywhere)
	float AttackRange;
    
	UPROPERTY(EditAnywhere)
	float SightRadius;

	UPROPERTY(EditAnywhere)
	float LoseSightRadius;

	UPROPERTY(EditAnywhere)
	float PeripheralVisionAngleDegrees;

	UPROPERTY(EditAnywhere)
	float TurnRate;
};

class OUTBREAK_API Define { };