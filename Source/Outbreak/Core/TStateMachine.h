// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TState.h"

template<class T>
class TState;

template<class T>
class TStateMachine
{
public:
	TStateMachine() = default;
	~TStateMachine() { StateMap.Empty(); }
	
	void AddState(T Key, TSharedPtr<TState<T>> State)
	{
		if (StateMap.Contains(Key))
		{
			UE_LOG(LogTemp, Error, TEXT("State %s already exists"), *UEnum::GetValueAsString(Key));
			return;
		}
		
		StateMap.Add(Key, State);
	}

	void ChangeState(T Key)
	{ 
		if (CurrentKey == Key) return;
		
		if (!StateMap.Contains(Key))
		{
			UE_LOG(LogTemp, Error, TEXT("State %s not found"), *UEnum::GetValueAsString(Key));
			return;
		}
		if (CurrentState.IsValid())
		{
			CurrentState->Exit(Key);
			PreviousState = CurrentState;
			PreviousKey = CurrentKey;
		}
		CurrentKey = Key;
		CurrentState = StateMap[Key];
		CurrentState->Enter(PreviousKey);
	}

	void Release()
	{
		StateMap.Empty();
		CurrentState.Reset();
		PreviousState.Reset();
	}

	void Execute(float DeltaTime)
	{
		CurrentState->Execute(CurrentKey, DeltaTime);
	}

	T GetCurrentState() const
	{
		if (CurrentState.IsValid())
		{
			return CurrentKey;
		}
		return T();
	}

	TSharedPtr<TState<T>> GetState(const T& Key) const
	{
		if (StateMap.Contains(Key))
		{
			return StateMap[Key];
		}
		return nullptr;
	}
	
	bool IsInState(T Key) const
	{
		return CurrentKey == Key;
	}

private:
	TMap<T, TSharedPtr<TState<T>>> StateMap;
	T CurrentKey;
	T PreviousKey;
	TSharedPtr<TState<T>> CurrentState;
	TSharedPtr<TState<T>> PreviousState;
};
