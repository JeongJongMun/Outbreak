// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSpawnManager.h"
#include "CharacterFactory.h"
#include "Outbreak/Util/EnumHelper.h"

ACharacterSpawnManager::ACharacterSpawnManager()
{
	CharacterFactory = CreateDefaultSubobject<UCharacterFactory>(TEXT("CharacterFactory"));
	static ConstructorHelpers::FObjectFinder<UDataTable> ZombieDataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Data/ZombieDataTable.ZombieDataTable'"));
	if (ZombieDataTableAsset.Succeeded())
	{
		ZombieDataTable = ZombieDataTableAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerDataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Data/PlayerDataTable.PlayerDataTable'"));
	if (PlayerDataTableAsset.Succeeded())
	{
		PlayerDataTable = PlayerDataTableAsset.Object;
	}
}

void ACharacterSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	LoadDataTableToMap(ZombieDataTable, ZombieDataMap);
	LoadDataTableToMap(PlayerDataTable, PlayerDataMap);

	// Test Spawn
	FCharacterSpawnParam FatZombieSpawnParam =
	{
		ECharacterType::Zombie,
		EPlayerType::None,
		EZombieSubType::Fat,
		FVector(100, 1500, 0),
		FRotator::ZeroRotator,
		1
	};

	FCharacterSpawnParam RunnerZombieSpawnParam =
	{
		ECharacterType::Zombie,
		EPlayerType::None,
		EZombieSubType::Runner,
		FVector(200, 1500, 0),
		FRotator::ZeroRotator,
		1
	};

	FCharacterSpawnParam WalkerZombieSpawnParam =
	{
		ECharacterType::Zombie,
		EPlayerType::None,
		EZombieSubType::Walker,
		FVector(300, 1500, 0),
		FRotator::ZeroRotator,
		1
	};

	FCharacterSpawnParam GymZombieSpawnParam =
	{
		ECharacterType::Zombie,
		EPlayerType::None,
		EZombieSubType::GymRat,
		FVector(400, 1500, 0),
		FRotator::ZeroRotator,
		1
	};
	
	// SpawnCharacter(FatZombieSpawnParam);
	// SpawnCharacter(RunnerZombieSpawnParam);
	// SpawnCharacter(WalkerZombieSpawnParam);
	// SpawnCharacter(GymZombieSpawnParam);
}

FZombieData* ACharacterSpawnManager::GetZombieData(const EZombieSubType Type)
{
	const FString RowName = EnumHelper::EnumToString(Type);
	if (ZombieDataMap.Contains(RowName))
	{
		return ZombieDataMap[RowName];
	}
	
	UE_LOG(LogTemp, Error, TEXT("[%s] No Zombie data found for type: %d"), CURRENT_CONTEXT, (int32)Type);
	return nullptr;
}

FPlayerData* ACharacterSpawnManager::GetPlayerData(const EPlayerType Type)
{
	const FString RowName = EnumHelper::EnumToString(Type);
	if (PlayerDataMap.Contains(RowName))
	{
		return PlayerDataMap[RowName];
	}

	UE_LOG(LogTemp, Error, TEXT("[%s] No Player data found for type: %d"), CURRENT_CONTEXT, (int32)Type);
	return nullptr;
}

void ACharacterSpawnManager::SpawnCharacter(const FCharacterSpawnParam& InSpawnParam) const
{
	for (int i = 0; i < InSpawnParam.SpawnCount; i++)
	{
		CharacterFactory->CreateCharacter(GetWorld(), InSpawnParam);
	}
}
