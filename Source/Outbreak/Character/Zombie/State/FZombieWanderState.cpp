#include "FZombieWanderState.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Outbreak/Character/Zombie/CharacterZombie.h"
#include "Outbreak/Util/Define.h"

void FZombieWanderState::Enter(EZombieState PreviousState)
{
	Super::Enter(PreviousState);
	
	if (Zombie && Zombie->GetCharacterMovement())
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = WanderSpeed;
	}

	WanderTimer = FMath::RandRange(MinWanderTime, MaxWanderTime);
	StartWandering();
	Zombie->PlayAnimation(EZombieAnimationType::Walk);
}

void FZombieWanderState::Execute(EZombieState CurrentState, float DeltaTime)
{
	Super::Execute(CurrentState, DeltaTime);
	
	EPathFollowingStatus::Type MoveStatus = Owner->GetMoveStatus();
	WanderTimer -= DeltaTime;
	if (WanderTimer <= 0.0f || MoveStatus == EPathFollowingStatus::Type::Idle)
	{
		float RandomValue = FMath::FRand();
		if (RandomValue < 0.3f)
		{
			ChangeState(EZombieState::Idle);
		}
		else
		{
			WanderTimer = FMath::RandRange(MinWanderTime, MaxWanderTime);
			StartWandering();
		}
	}

	// TODO : Player Detection Logic
	
}

void FZombieWanderState::Exit(EZombieState NextState)
{
	Super::Exit(NextState);
	
	Owner->StopMovement();
}

void FZombieWanderState::StartWandering()
{
	if (!Zombie)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Zombie is nullptr"), CURRENT_CONTEXT);
		return;
	}
    
	FVector RandomLocation;
	if (FindRandomWanderLocation(RandomLocation))
	{
		Owner->MoveToLocation(RandomLocation, -1.0f, true);
	}
}

bool FZombieWanderState::FindRandomWanderLocation(FVector& OutLocation)
{
	if (!Zombie)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Zombie is nullptr"), CURRENT_CONTEXT);
		return false;
	}
    
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner->GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Navigation system is nullptr"), CURRENT_CONTEXT);
		return false;
	}
    
	FVector Origin = Zombie->GetActorLocation();
    
	FNavLocation RandomPoint;
	bool bFound = NavSystem->GetRandomReachablePointInRadius(Origin, WanderRadius, RandomPoint);
    
	if (bFound)
	{
		OutLocation = RandomPoint.Location;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] No random point found in radius"), CURRENT_CONTEXT);
	return false;
}