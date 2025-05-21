// AmmoBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class OUTBREAK_API AAmmoBase : public AActor
{
	GENERATED_BODY()

public:
	AAmmoBase();



protected:

};
