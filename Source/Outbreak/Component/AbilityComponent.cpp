#include "AbilityComponent.h"

#include "Outbreak/Ability/ActiveAbilityObject.h"
#include "Outbreak/Ability/PassiveAbilityObject.h"
#include "Outbreak/Character/CharacterBase.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeAbilities();
}

void UAbilityComponent::InitializeAbilities()
{
	for (UBaseAbilityObject* Ability : Abilities)
	{
		if (!Ability) continue;

		Ability->SetOwnerCharacter(Cast<ACharacterBase>(GetOwner()));
		AbilityMap.Add(Ability->GetAbilityType(), Ability);

		if (UPassiveAbilityObject* Passive = Cast<UPassiveAbilityObject>(Ability))
		{
			Passive->UseAbility();
		}
	}
}

bool UAbilityComponent::TryUseAbility(const EAbilityType Type) const
{
	if (UActiveAbilityObject* Ability = Cast<UActiveAbilityObject>(GetAbility(Type)))
	{
		if (Ability->CanUseAbility())
		{
			return Ability->UseAbility();
		}
	}
	return false;
}

void UAbilityComponent::AddAbility(UBaseAbilityObject* NewAbility)
{
	if (!NewAbility) return;

	Abilities.Add(NewAbility);
	NewAbility->SetOwnerCharacter(Cast<ACharacterBase>(GetOwner()));
	AbilityMap.Add(NewAbility->GetAbilityType(), NewAbility);

	if (UPassiveAbilityObject* Passive = Cast<UPassiveAbilityObject>(NewAbility))
	{
		Passive->UseAbility();
	}
}

void UAbilityComponent::RemoveAbility(EAbilityType Type)
{
	if (const TObjectPtr<UBaseAbilityObject> Ability = GetAbility(Type))
	{
		Abilities.Remove(Ability);
		AbilityMap.Remove(Type);
	}
}

TObjectPtr<UBaseAbilityObject> UAbilityComponent::GetAbility(const EAbilityType Type) const
{
	if (AbilityMap.Contains(Type))
	{
		return AbilityMap[Type];
	}
	return nullptr;
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UpdateCooldowns(DeltaTime);
	// UpdateDurations(DeltaTime);
}

void UAbilityComponent::UpdateCooldowns(float DeltaTime)
{
	for (TObjectPtr Ability : Abilities)
	{
		if (!Ability || !IsValid(Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] Ability is not valid."), CURRENT_CONTEXT);
			continue;
		}
		
		if (const TObjectPtr<UActiveAbilityObject> Active = Cast<UActiveAbilityObject>(Ability))
		{
			Active->TickCooldown(DeltaTime);
		}
	}
}

void UAbilityComponent::UpdateDurations(float DeltaTime)
{
	for (UBaseAbilityObject* Ability : Abilities)
	{
		if (!Ability || !IsValid(Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] Ability is not valid."), CURRENT_CONTEXT);
			continue;
		}
		
		if (const TObjectPtr<UActiveAbilityObject> Active = Cast<UActiveAbilityObject>(Ability))
		{
			Active->TickDuration(DeltaTime);
		}
	}
}