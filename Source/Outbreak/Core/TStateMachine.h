// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TState.h"

template<typename T, typename ContextType = UObject>
class TStateMachine
{
public:
	TStateMachine() = default;
	virtual ~TStateMachine() { StateMap.Empty(); }
	
	virtual void AddState(T Key, TSharedPtr<TState<T, ContextType>> State)
	{
		if (StateMap.Contains(Key))
		{
			UE_LOG(LogTemp, Error, TEXT("State %s already exists"), *UEnum::GetValueAsString(Key));
			return;
		}
		
		StateMap.Add(Key, State);
	}

	virtual void ChangeState(T Key, TObjectPtr<ContextType> Context = nullptr)
	{ 
		if (CurrentKey == Key) return;
		
		if (!StateMap.Contains(Key))
		{
			UE_LOG(LogTemp, Error, TEXT("State %s not found"), *UEnum::GetValueAsString(Key));
			return;
		}
		if (CurrentState.IsValid())
		{
			CurrentState->Exit(Key, Context);
			PreviousState = CurrentState;
			PreviousKey = CurrentKey;
		}
		CurrentKey = Key;
		CurrentState = StateMap[Key];
		CurrentState->Enter(PreviousKey, Context);
	}

	virtual void Release()
	{
		StateMap.Empty();
		CurrentState.Reset();
		PreviousState.Reset();
	}

	virtual void Execute(float DeltaTime)
	{
		CurrentState->Execute(CurrentKey, DeltaTime);
	}

	virtual T GetCurrentState() const
	{
		if (CurrentState.IsValid())
		{
			return CurrentKey;
		}
		return T();
	}

	virtual TSharedPtr<TState<T, ContextType>> GetState(const T& Key) const
	{
		if (StateMap.Contains(Key))
		{
			return StateMap[Key];
		}
		return nullptr;
	}
	
	virtual bool IsInState(T Key) const
	{
		return CurrentKey == Key;
	}

private:
	TMap<T, TSharedPtr<TState<T, ContextType>>> StateMap;
	T CurrentKey;
	T PreviousKey;
	TSharedPtr<TState<T, ContextType>> CurrentState;
	TSharedPtr<TState<T, ContextType>> PreviousState;
};
