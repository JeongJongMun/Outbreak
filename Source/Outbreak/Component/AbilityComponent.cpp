#include "AbilityComponent.h"
#include "Outbreak/Character/CharacterBase.h"
#include "Outbreak/Game/Ability/ActiveAbilityObject.h"
#include "Outbreak/Game/Ability/PassiveAbilityObject.h"
#include "Outbreak/Util/UObjectHelper.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
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

void UAbilityComponent::AddAbility(const TObjectPtr<UBaseAbilityObject>& NewAbility)
{
	if (!UObjectHelper::IsUObjectValid(NewAbility))
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] NewAbility is not valid."), CURRENT_CONTEXT);
		return;
	}

	const EAbilityType Type = NewAbility->GetAbilityType();

	NewAbility->SetOwnerCharacter(Cast<ACharacterBase>(GetOwner()));
	AbilityArray.Add(NewAbility);
	AbilityMap.Add(Type, NewAbility);

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

	UE_LOG(LogTemp, Warning, TEXT("[%s] Ability of type %s not found."), CURRENT_CONTEXT, *EnumHelper::EnumToString(Type));
	return nullptr;
}

void UAbilityComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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