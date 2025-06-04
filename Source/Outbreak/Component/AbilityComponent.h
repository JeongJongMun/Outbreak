#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Outbreak/Ability/BaseAbility.h"
#include "AbilityComponent.generated.h"

UCLASS()
class OUTBREAK_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeAbilities();

	bool TryUseAbility(EAbilityType Type) const;

	void AddAbility(class UBaseAbilityObject* NewAbility);
	void RemoveAbility(EAbilityType Type);

	TObjectPtr<UBaseAbilityObject> GetAbility(EAbilityType Type) const;

protected:
	virtual void BeginPlay() override;

private:
	TArray<UBaseAbilityObject*> Abilities;
	TMap<EAbilityType, TObjectPtr<UBaseAbilityObject>> AbilityMap;

	void UpdateCooldowns(float DeltaTime);
	void UpdateDurations(float DeltaTime);
};
