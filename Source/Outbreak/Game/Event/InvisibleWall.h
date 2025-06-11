#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "InvisibleWall.generated.h"

UCLASS()
class OUTBREAK_API AInvisibleWall : public AActor
{
	GENERATED_BODY()

public:
	AInvisibleWall();

	UPROPERTY(EditInstanceOnly)
	UBoxComponent* RightVerticalWall;
	
	UPROPERTY(EditInstanceOnly)
	UBoxComponent* LeftVerticalWall;
	
	UPROPERTY(EditInstanceOnly)
	UBoxComponent* HorizontalWall;

	UFUNCTION()
	void DisableWall();
	
protected:
	virtual void BeginPlay() override;
	
};
