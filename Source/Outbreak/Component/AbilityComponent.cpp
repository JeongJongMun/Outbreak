#include "AbilityComponent.h"
#include "Outbreak/Ability/ActiveAbilityObject.h"
#include "Outbreak/Ability/PassiveAbilityObject.h"
#include "Outbreak/Character/CharacterBase.h"
#include "Outbreak/Util/UObjectHelper.h"

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
	for (TObjectPtr Ability : AbilityArray)
	{
		if (!Ability) continue;

		Ability->SetOwnerCharacter(Cast<ACharacterBase>(GetOwner()));
		AbilityMap.Add(Ability->GetAbilityType(), Ability);

		if (const TObjectPtr<UPassiveAbilityObject> Passive = Cast<UPassiveAbilityObject>(Ability))
		{
			Passive->UseAbility();
		}
	}
	
	BIsInit = true;
}

bool UAbilityComponent::TryUseAbility(const EAbilityType Type) const
{
	if (const TObjectPtr<UActiveAbilityObject> Ability = Cast<UActiveAbilityObject>(GetAbility(Type)))
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

	AbilityArray.Add(NewAbility);
	NewAbility->SetOwnerCharacter(Cast<ACharacterBase>(GetOwner()));
	AbilityMap.Add(NewAbility->GetAbilityType(), NewAbility);

	if (const TObjectPtr<UPassiveAbilityObject> Passive = Cast<UPassiveAbilityObject>(NewAbility))
	{
		Passive->UseAbility();
	}
}

void UAbilityComponent::RemoveAbility(const EAbilityType Type)
{
	if (const TObjectPtr<UBaseAbilityObject> Ability = GetAbility(Type))
	{
		AbilityArray.Remove(Ability);
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

void UAbilityComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!BIsInit)
		return;
	
	UpdateCooldowns(DeltaTime);
	UpdateDurations(DeltaTime);
}

void UAbilityComponent::UpdateCooldowns(const float DeltaTime)
{
	for (TObjectPtr Ability : AbilityArray)
	{
		if (!UObjectHelper::IsUObjectValid(Ability))
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

void UAbilityComponent::UpdateDurations(const float DeltaTime)
{
	for (TObjectPtr Ability : AbilityArray)
	{
		if (!UObjectHelper::IsUObjectValid(Ability))
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