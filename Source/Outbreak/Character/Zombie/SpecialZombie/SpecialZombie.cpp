// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecialZombie.h"
#include "Outbreak/Component/AbilityComponent.h"

ASpecialZombie::ASpecialZombie()
{
	ZombieType = EZombieType::Special;
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
}

void ASpecialZombie::BeginPlay()
{
	Super::BeginPlay();
}
