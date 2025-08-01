// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterFactory.h"

#include "Outbreak/Character/Zombie/NormalZombie/FatZombie.h"
#include "Outbreak/Character/Zombie/NormalZombie/RunnerZombie.h"
#include "Outbreak/Character/Zombie/NormalZombie/WalkerZombie.h"
#include "Outbreak/Character/Zombie/SpecialZombie/GymRatZombie.h"
#include "Outbreak/Util/EnumHelper.h"

UCharacterFactory::UCharacterFactory()
{
    InitializeFactoryMaps();
}

ACharacterBase* UCharacterFactory::CreateCharacter(UWorld* InWorld, const FCharacterSpawnParam& InSpawnParam)
{
    const TSubclassOf<ACharacterBase> ACharacterClass = GetCharacterClassFromType(InSpawnParam);

    if (ACharacterClass && InWorld)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        if (ACharacterBase* SpawnedCharacter = InWorld->SpawnActor<ACharacterBase>(ACharacterClass, InSpawnParam.SpawnLocation, InSpawnParam.SpawnRotation, SpawnParams))
        {
            UE_LOG(LogTemp, Log, TEXT("[%s] %s Character Spawned"), CURRENT_CONTEXT, *EnumHelper::EnumToString(InSpawnParam.CharacterType));
            return SpawnedCharacter;
        }
    }
    UE_LOG(LogTemp, Error, TEXT("[%s] %s Character Spawn Failed"), CURRENT_CONTEXT, *EnumHelper::EnumToString(InSpawnParam.CharacterType));
    return nullptr;
}

void UCharacterFactory::InitializeFactoryMaps()
{
    ZombieClassMap.Add(EZombieSubType::Walker, AWalkerZombie::StaticClass());
    ZombieClassMap.Add(EZombieSubType::Runner, ARunnerZombie::StaticClass());
    ZombieClassMap.Add(EZombieSubType::Fat, AFatZombie::StaticClass());
    // ZombieClassMap.Add(EZombieSubType::Soldier, ASoldierZombie::StaticClass());
    ZombieClassMap.Add(EZombieSubType::GymRat, AGymRatZombie::StaticClass());
    // ZombieClassMap.Add(EZombieSubType::Radioactive, ARadioactiveZombie::StaticClass());
    // ZombieClassMap.Add(EZombieSubType::Ghost, AGhostZombie::StaticClass());
    // ZombieClassMap.Add(EZombieSubType::Shield, AShieldZombie::StaticClass());
    // ZombieClassMap.Add(EZombieSubType::Berserker, ABerserkerZombie::StaticClass());

    // PlayerClassMap.Add(EPlayerType::Player1, APlayerCharacter1::StaticClass());
    // PlayerClassMap.Add(EPlayerType::Player2, APlayerCharacter2::StaticClass());
    // PlayerClassMap.Add(EPlayerType::Player3, APlayerCharacter3::StaticClass());
    // PlayerClassMap.Add(EPlayerType::Player4, APlayerCharacter4::StaticClass());
}

TSubclassOf<ACharacterBase> UCharacterFactory::GetCharacterClassFromType(const FCharacterSpawnParam& InSpawnParam)
{
    switch (InSpawnParam.CharacterType)
    {
        case ECharacterType::Zombie:
            {
                if (ZombieClassMap.Contains(InSpawnParam.ZombieSubType))
                {
                    return ZombieClassMap[InSpawnParam.ZombieSubType];
                }
                break;
            }

        case ECharacterType::Player:
            {
                if (PlayerClassMap.Contains(InSpawnParam.PlayerType))
                {
                    return PlayerClassMap[InSpawnParam.PlayerType];
                }
                break;
            }

        default:
            break;
    }

    UE_LOG(LogTemp, Error, TEXT("[%s] Unknown character class for type [%d]"), CURRENT_CONTEXT, (int32)InSpawnParam.CharacterType);
    return nullptr;
}
