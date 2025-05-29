// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieFactory.h"
#include "NormalZombie.h"
#include "RunnerZombie.h"
#include "WalkerZombie.h"

ACharacterZombie* UZombieFactory::CreateZombie(UWorld* World, const EZombieSubType ZombieSubType,
                                               const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    TSubclassOf<ACharacterZombie> ZombieClass = GetZombieClassFromSubType(ZombieSubType);

    if (ZombieClass && World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        ACharacterZombie* SpawnedZombie = World->SpawnActor<ACharacterZombie>(ZombieClass, SpawnLocation, SpawnRotation, SpawnParams);
        
        UE_LOG(LogTemp, Log, TEXT("[%s] Zombie Spawned. SubType: %s"), CURRENT_CONTEXT, *UEnum::GetValueAsString(ZombieSubType));
        
        return SpawnedZombie;
    }

    UE_LOG(LogTemp, Log, TEXT("[%s] Zombie Spawn Failed"), CURRENT_CONTEXT);
    return nullptr;
}

TSubclassOf<ACharacterZombie> UZombieFactory::GetZombieClassFromSubType(EZombieSubType ZombieSubType) const
{
    TSubclassOf<ACharacterZombie> ZombieClass;
    
    switch (ZombieSubType)
    {
        case EZombieSubType::Walker:
            ZombieClass = AWalkerZombie::StaticClass();
            break;
        case EZombieSubType::Runner:
            ZombieClass = ARunnerZombie::StaticClass();
            break;
        // case EZombieSubType::Fat:
        //     ZombieClass = AFatZombie::StaticClass();
        //     break;
        // case EZombieSubType::Soldier:
        //     ZombieClass = ASoldierZombie::StaticClass();
        //     break;
        
        // case EZombieSubType::GymRat:
        //     ZombieClass = AGymRatZombie::StaticClass();
        //     break;
        // case EZombieSubType::Radioactive:
        //     ZombieClass = ARadioactiveZombie::StaticClass();
        //     break;
        // case EZombieSubType::Ghost:
        //     ZombieClass = AGhostZombie::StaticClass();
        //     break;
        // case EZombieSubType::Shield:
        //     ZombieClass = AShieldZombie::StaticClass();
        //     break;
        // case EZombieSubType::Berserker:
        //     ZombieClass = ABerserkerZombie::StaticClass();
        //     break;
        default:
            ZombieClass = ACharacterZombie::StaticClass();
            break;
    }
    
    return ZombieClass;
}
