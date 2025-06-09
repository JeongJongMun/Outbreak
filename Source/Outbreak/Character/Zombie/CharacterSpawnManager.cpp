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
	
	SpawnCharacter(FatZombieSpawnParam);
	SpawnCharacter(RunnerZombieSpawnParam);
	SpawnCharacter(WalkerZombieSpawnParam);
	SpawnCharacter(GymZombieSpawnParam);
}

FCharacterBaseData* ACharacterSpawnManager::GetCharacterData(ECharacterType CharacterType, const FString& RowName)
{
	if (CharacterType == ECharacterType::Zombie)
	{
		if (ZombieDataMap.Contains(RowName))
		{
			FZombieData* ZombieData = ZombieDataMap[RowName];
			return ZombieData;
		}
	}
	else if (CharacterType == ECharacterType::Player)
	{
		if (PlayerDataMap.Contains(RowName))
		{
			FPlayerData* PlayerData = PlayerDataMap[RowName];
			return PlayerData;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("[%s] No data found for type [%d], row [%s]"), CURRENT_CONTEXT, (int32)CharacterType, *RowName);
	return nullptr;
}

void ACharacterSpawnManager::SpawnCharacter(const FCharacterSpawnParam& InSpawnParam)
{
	FString RowName;
	switch (InSpawnParam.CharacterType)
	{
		case ECharacterType::Player:
			RowName = EnumHelper::EnumToString(InSpawnParam.PlayerType);
			break;
		case ECharacterType::Zombie:
			RowName = EnumHelper::EnumToString(InSpawnParam.ZombieSubType);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("[%s] Invalid Character Type: %d"), CURRENT_CONTEXT, (int32)InSpawnParam.CharacterType);
			return;
	}
	FCharacterBaseData* CharacterData = GetCharacterData(InSpawnParam.CharacterType, RowName);
	if (!CharacterData)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to get character data for type: %d, row: %s"), CURRENT_CONTEXT, (int32)InSpawnParam.CharacterType, *RowName);
		return;
	}

	for (int i = 0; i < InSpawnParam.SpawnCount; i++)
	{
		TObjectPtr<ACharacterBase> SpawnedCharacter = CharacterFactory->CreateCharacter(GetWorld(), InSpawnParam);
		switch (InSpawnParam.CharacterType)
		{
			case ECharacterType::Player:
			{
				TObjectPtr<ACharacterPlayer> Player = CastChecked<ACharacterPlayer>(SpawnedCharacter);
				FPlayerData* PlayerData = static_cast<FPlayerData*>(CharacterData);
				Player->InitializePlayerData(PlayerData);
				break;
			}
			case ECharacterType::Zombie:
			{
				TObjectPtr<ACharacterZombie> Zombie = CastChecked<ACharacterZombie>(SpawnedCharacter);
				FZombieData* ZombieData = static_cast<FZombieData*>(CharacterData);
				Zombie->InitializeZombieData(ZombieData);
				break;
			}
			default:
				break;
		}
	}
}