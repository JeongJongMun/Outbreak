// Fill out your copyright notice in the Description page of Project Settings.

#include "FZombieStateBase.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

FZombieStateBase::~FZombieStateBase()
{
}

void FZombieStateBase::Enter(EZombieState PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
}

void FZombieStateBase::Execute(EZombieState CurrentState, float DeltaTime)
{
}

void FZombieStateBase::Exit(EZombieState NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	if (Zombie->GetMesh() && Zombie->GetMesh()->IsPlaying())
	{
		Zombie->GetMesh()->Stop();
	}
}