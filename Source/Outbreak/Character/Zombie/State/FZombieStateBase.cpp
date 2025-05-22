// Fill out your copyright notice in the Description page of Project Settings.


#include "FZombieStateBase.h"

#include "Kismet/GameplayStatics.h"

FZombieStateBase::~FZombieStateBase()
{
}

void FZombieStateBase::Enter(EZombieState PreviousState)
{
}

void FZombieStateBase::Execute(EZombieState CurrentState, float DeltaTime)
{
	if (FindTargetPlayer())
	{
		ChangeState(EZombieState::Chase);
	}
}

void FZombieStateBase::Exit(EZombieState NextState)
{
	if (Zombie->GetMesh() && Zombie->GetMesh()->IsPlaying())
	{
		Zombie->GetMesh()->Stop();
	}
}

bool FZombieStateBase::FindTargetPlayer()
{
	if (!Owner || !Zombie) return false;

	const UWorld* World = Zombie->GetWorld();
	if (!World) return false;

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(World, ACharacterBase::StaticClass(), Players);

	for (AActor* Actor : Players)
	{
		if (!Actor || Actor == Zombie) continue;

		const float DistSq = FVector::DistSquared(Zombie->GetActorLocation(), Actor->GetActorLocation());
		// if (DistSq > AggroDistance * AggroDistance) continue;
		
		if (DistSq < NearestDistanceSq)
		{
			NearestDistanceSq = DistSq;
			CurrentTarget = Cast<ACharacterPlayer>(Actor);
		}
	}
	
	if (CurrentTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] Found target player: %s"), CURRENT_CONTEXT, *CurrentTarget->GetName());
		return true;
	}
	
	return false;
}
