// AmmoBase.cpp
#include "AmmoBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAmmoBase::AAmmoBase()
{
	PrimaryActorTick.bCanEverTick = false;

}
