// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Outbreak/Data/GameData.h"
#include "Outbreak/Util/Define.h"
#include "CharacterSpawnManager.generated.h"

struct FCharacterSpawnParam
{
	ECharacterType CharacterType;
	EPlayerType PlayerType;
	EZombieSubType ZombieSubType;
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	int32 SpawnCount = 1;
};

UCLASS()
class OUTBREAK_API ACharacterSpawnManager : public AActor
{
	GENERATED_BODY()

// --------------------
// Functions
// --------------------
public:	
	ACharacterSpawnManager();
	// TODO : After Spawn 'Player', Need to decide who will be possessing player
	void SpawnCharacter(const FCharacterSpawnParam& InSpawnParam) const;
	FZombieData* GetZombieData(const EZombieSubType Type);
	FPlayerData* GetPlayerData(const EPlayerType Type);
	
	void SetSettingId(FName InSettingId);
	void SetWaveId(FName InWaveId);

protected:
	virtual void BeginPlay() override;

private:
	bool GetSettingDataFromDataTable(FName InSettingsID, FSpawnerSettingData& OutSetting);
	bool GetWaveDataFromDataTable(FName InWaveId, FWaveData& OutWaveData);
	FSpawnerSettingData GetCurrentSettingData() { return SpawnerSettingData; }
	FWaveData GetCurrentWaveData() { return WaveData; }
	void UpdateSettingData();
	void UpdateWaveData();

	void ClampSettingDataValues(FSpawnerSettingData& Setting);

	// TODO : Move to utility class
	template <typename T>
	void LoadDataTableToMap(const TObjectPtr<UDataTable> DataTable, TMap<FString, T*>& OutMap)
	{
		if (!DataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] DataTable is null!"), CURRENT_CONTEXT);
			return;
		}

		const TMap<FName, uint8*> RowMap = DataTable->GetRowMap();

		for (const TPair<FName, uint8*>& Row : RowMap)
		{
			if (T* TypedRow = reinterpret_cast<T*>(Row.Value))
			{
				OutMap.Add(Row.Key.ToString(), TypedRow);
			}
		}
	}


// --------------------
// Variables
// --------------------
private:
	UPROPERTY()
	TObjectPtr<UDataTable> SpawnerSettingDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> WaveDataTable;

	FSpawnerSettingData SpawnerSettingData;
	FWaveData WaveData;
	
	FName SpawnerSettingId = FName("Default");
	FName WaveId = FName("0");
	
	UPROPERTY()
	TObjectPtr<class UCharacterFactory> CharacterFactory;
	
	UPROPERTY()
	TObjectPtr<UDataTable> ZombieDataTable;
	
	UPROPERTY()
	TObjectPtr<UDataTable> PlayerDataTable;
	
	TMap<FString, FZombieData*> ZombieDataMap;
	TMap<FString, FPlayerData*> PlayerDataMap;
};