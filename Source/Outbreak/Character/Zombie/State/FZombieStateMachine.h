#pragma once
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Core/TStateMachine.h"

enum class EZombieState : uint8;

class FZombieStateMachine : public TStateMachine<EZombieState, ACharacterPlayer>
{
public:
	
};
