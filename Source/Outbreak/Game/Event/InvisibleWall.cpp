#include "InvisibleWall.h"

AInvisibleWall::AInvisibleWall()
{
	PrimaryActorTick.bCanEverTick = false;

	LeftVerticalWall = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftVerticalWall"));
	LeftVerticalWall->SetupAttachment(RootComponent);
	LeftVerticalWall->SetBoxExtent(FVector(50.f, 10.f, 200.f));
	LeftVerticalWall->SetRelativeLocation(FVector(0.f, 0.f, 200.f)); 
	LeftVerticalWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftVerticalWall->SetCollisionResponseToAllChannels(ECR_Block);
	LeftVerticalWall->SetHiddenInGame(true);

	RightVerticalWall = CreateDefaultSubobject<UBoxComponent>(TEXT("RightVerticalWall"));
	RightVerticalWall->SetupAttachment(RootComponent);
	RightVerticalWall->SetBoxExtent(FVector(50.f, 10.f, 200.f));
	RightVerticalWall->SetRelativeLocation(FVector(400.f, 0.f, 200.f));
	RightVerticalWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightVerticalWall->SetCollisionResponseToAllChannels(ECR_Block);
	RightVerticalWall->SetHiddenInGame(true);
	
	HorizontalWall = CreateDefaultSubobject<UBoxComponent>(TEXT("HorizontalWall"));
	HorizontalWall->SetupAttachment(RootComponent);
	HorizontalWall->SetBoxExtent(FVector(200.f, 10.f, 50.f));
	HorizontalWall->SetRelativeLocation(FVector(200.f, 0.f, 400.f));
	HorizontalWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HorizontalWall->SetCollisionResponseToAllChannels(ECR_Block);
	HorizontalWall->SetHiddenInGame(true);
}

void AInvisibleWall::BeginPlay()
{
	Super::BeginPlay();
}

void AInvisibleWall::DisableWall()
{
	LeftVerticalWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightVerticalWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HorizontalWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

