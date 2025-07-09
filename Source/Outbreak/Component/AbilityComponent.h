#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Outbreak/Game/Ability/BaseAbility.h"
#include "AbilityComponent.generated.h"

UCLASS()
class OUTBREAK_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool TryUseAbility(EAbilityType Type) const;

	void AddAbility(const TObjectPtr<class UBaseAbilityObject>& NewAbility);
	void RemoveAbility(EAbilityType Type);

	TObjectPtr<UBaseAbilityObject> GetAbility(EAbilityType Type) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UBaseAbilityObject>> AbilityArray;
	
	UPROPERTY(VisibleAnywhere)
	TMap<EAbilityType, TObjectPtr<UBaseAbilityObject>> AbilityMap;

	void UpdateCooldowns(float DeltaTime);
	void UpdateDurations(float DeltaTime);
};
