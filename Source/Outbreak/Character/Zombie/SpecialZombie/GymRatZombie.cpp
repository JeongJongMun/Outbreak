// Fill out your copyright notice in the Description page of Project Settings.


#include "GymRatZombie.h"

#include "Outbreak/Ability/DamageEnhanceAbility.h"
#include "Outbreak/Ability/VibrationAbility.h"
#include "Outbreak/Component/AbilityComponent.h"

AGymRatZombie::AGymRatZombie()
{
}

void AGymRatZombie::BeginPlay()
{
	SetMesh(ECharacterBodyType::Muscle);
	SetPhysicalAsset(ECharacterType::Zombie, ECharacterBodyType::Muscle);
	
	Super::BeginPlay();

	AbilityComponent->AddAbility(NewObject<UDamageEnhanceAbility>(AbilityComponent));
	AbilityComponent->AddAbility(NewObject<UVibrationAbility>(AbilityComponent));

	SetActorScale3D(FVector(BodyScale, BodyScale, BodyScale));
}
