#pragma once
#include "Define.h"
#include "EnumHelper.h"

UENUM()
enum class EUObjectValidationFailureType : uint8
{
	None = 0,
	NullPointer,
	BeingDestroyed,
	InvalidLowLevel,
	InvalidLowLevelFast,
	InvalidGeneral,
	InvalidClass,
};

struct FUObjectValidationResult
{
	bool bIsValid = false;
	EUObjectValidationFailureType FailureReason = EUObjectValidationFailureType::None;
	FUObjectValidationResult(const bool InIsValid, const EUObjectValidationFailureType InFailureReason = EUObjectValidationFailureType::None)
		: bIsValid(InIsValid), FailureReason(InFailureReason) {}

	operator bool() const { return bIsValid; }
};

class UObjectHelper
{
public:
    static FUObjectValidationResult ValidateUObject(const TObjectPtr<UObject>& Object, const bool bLogFailures = true)
    {
        if (!Object)
        {
            if (bLogFailures)
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] Object is null"), CURRENT_CONTEXT);
            }
            return FUObjectValidationResult(false, EUObjectValidationFailureType::NullPointer);
        }

        if (Object->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
        {
            if (bLogFailures)
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] Object is in the process of being destroyed"), CURRENT_CONTEXT);
            }
            return FUObjectValidationResult(false, EUObjectValidationFailureType::BeingDestroyed);
        }

        if (!Object->IsValidLowLevel())
        {
            if (bLogFailures)
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] Object is not valid at low level"), CURRENT_CONTEXT);
            }
            return FUObjectValidationResult(false, EUObjectValidationFailureType::InvalidLowLevel);
        }

        if (!Object->IsValidLowLevelFast())
        {
            if (bLogFailures)
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] Object is not valid at low level fast"), CURRENT_CONTEXT);
            }
            return FUObjectValidationResult(false, EUObjectValidationFailureType::InvalidLowLevelFast);
        }

        if (!IsValid(Object))
        {
            if (bLogFailures)
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] Object is not valid"), CURRENT_CONTEXT);
            }
            return FUObjectValidationResult(false, EUObjectValidationFailureType::InvalidGeneral);
        }

        if (!Object->GetClass() || !IsValid(Object->GetClass()))
        {
            if (bLogFailures)
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] Object class is invalid"), CURRENT_CONTEXT);
            }
            return FUObjectValidationResult(false, EUObjectValidationFailureType::InvalidClass);
        }

        return FUObjectValidationResult(true, EUObjectValidationFailureType::None);
    }

    static bool IsUObjectValid(const TObjectPtr<UObject>& Object, const bool bLogFailures = true)
    {
        return ValidateUObject(Object, bLogFailures).bIsValid;
    }

    static FString GetFailureReasonString(const EUObjectValidationFailureType FailureReason)
    {
        return EnumHelper::EnumToString(FailureReason);
    }
};
