// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawnManager.h"
#include "ZombieFactory.h"

AZombieSpawnManager::AZombieSpawnManager()
{
	ZombieFactory = CreateDefaultSubobject<UZombieFactory>(TEXT("ZombieFactory"));
	static ConstructorHelpers::FObjectFinder<UDataTable> ZombieDataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Data/ZombieDataTable.ZombieDataTable'"));
	if (ZombieDataTableAsset.Succeeded())
	{
		ZombieDataTable = ZombieDataTableAsset.Object;
	}
}

void AZombieSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnZombie(EZombieSubType::Runner, 3, FVector(0, 500, 0), FRotator::ZeroRotator);
}

FZombieData* AZombieSpawnManager::GetZombieData(EZombieSubType InZombieSubType) const
{
	FString EnumValueString = UEnum::GetValueAsString(InZombieSubType);
	FString RowName;
	EnumValueString.Split(TEXT("::"), nullptr, &RowName);
	
	FZombieData* ZombieData = ZombieDataTable->FindRow<FZombieData>(*RowName, TEXT(""));
	if (!ZombieData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ZombieData not found for subtype: %s"), CURRENT_CONTEXT, *RowName);
		return nullptr;
	}
	
	return ZombieData;
}

void AZombieSpawnManager::SpawnZombie(EZombieSubType InZombieSubType, int Count, const FVector& SpawnLocation,
	const FRotator& SpawnRotation)
{
	UWorld* World = GetWorld();
	FZombieData* SpawnedZombieData = GetZombieData(InZombieSubType);
	
	for (int i = 0; i < Count; i++)
	{
		TObjectPtr<ACharacterZombie> SpawnedZombie = ZombieFactory->CreateZombie(World, InZombieSubType, SpawnLocation, SpawnRotation);
		SpawnedZombie->InitializeZombieData(SpawnedZombieData);
	}
}
