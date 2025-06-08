// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void SpawnCharacter(const FCharacterSpawnParam& InSpawnParam);

protected:
	virtual void BeginPlay() override;

private:
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

	FCharacterBaseData* GetCharacterData(ECharacterType CharacterType, const FString& RowName);

// --------------------
// Variables
// --------------------
private:
	TObjectPtr<class UCharacterFactory> CharacterFactory;
	TObjectPtr<UDataTable> ZombieDataTable;
	TObjectPtr<UDataTable> PlayerDataTable;
	TMap<FString, FZombieData*> ZombieDataMap;
	TMap<FString, FPlayerData*> PlayerDataMap;

};