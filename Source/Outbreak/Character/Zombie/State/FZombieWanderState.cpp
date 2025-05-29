#include "FZombieWanderState.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

void FZombieWanderState::Enter(EZombieStateType PreviousState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Enter(PreviousState, TargetPlayer);
	
	if (Zombie && Zombie->GetCharacterMovement())
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = WanderSpeed;
	}

	WanderTimer = FMath::RandRange(MinWanderTime, MaxWanderTime);
	StartWandering();
}

void FZombieWanderState::Execute(EZombieStateType CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	
	EPathFollowingStatus::Type MoveStatus = Owner->GetMoveStatus();
	WanderTimer -= DeltaTime;
	if (WanderTimer <= 0.0f || MoveStatus == EPathFollowingStatus::Type::Idle)
	{
		float RandomValue = FMath::FRand();
		if (RandomValue < 0.3f)
		{
			ChangeState(EZombieStateType::Idle);
		}
		else
		{
			WanderTimer = FMath::RandRange(MinWanderTime, MaxWanderTime);
			StartWandering();
		}
	}
}

void FZombieWanderState::Exit(EZombieStateType NextState, TObjectPtr<ACharacterPlayer> TargetPlayer)
{
	Super::Exit(NextState, TargetPlayer);
	
	Owner->StopMovement();
}

void FZombieWanderState::StartWandering()
{
	if (!Zombie)
		return;
    
	FVector RandomLocation;
	if (FindRandomWanderLocation(RandomLocation))
	{
		Owner->MoveToLocation(RandomLocation, -1.0f, true);
	}
}

bool FZombieWanderState::FindRandomWanderLocation(FVector& OutLocation)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner->GetWorld());
	if (!Zombie || !NavSystem)
		return false;
    
	FVector OriginPosition = Zombie->GetActorLocation();
    
	FNavLocation RandomPoint;
	bool bFound = NavSystem->GetRandomReachablePointInRadius(OriginPosition, WanderRadius, RandomPoint);
    
	if (bFound)
	{
		OutLocation = RandomPoint.Location;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] No random point found in radius"), CURRENT_CONTEXT);
	return false;
}