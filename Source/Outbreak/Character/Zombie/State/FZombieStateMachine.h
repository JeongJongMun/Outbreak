﻿#pragma once
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Core/TStateMachine.h"

enum class EZombieStateType : uint8;

class FZombieStateMachine : public TStateMachine<EZombieStateType, ACharacterPlayer>
{
public:
	virtual void ChangeState(EZombieStateType Key, TObjectPtr<ACharacterPlayer> Context = nullptr) override;
};