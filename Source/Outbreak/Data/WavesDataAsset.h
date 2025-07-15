#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "Engine/DataAsset.h"
#include "WavesDataAsset.generated.h"

UCLASS(BlueprintType)
class OUTBREAK_API UWavesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FWavesData WavesData;
};
