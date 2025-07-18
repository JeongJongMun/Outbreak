// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyZombieAttackEnd.h"

#include "Outbreak/Character/Zombie/CharacterZombie.h"

void UAnimNotifyZombieAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ACharacterZombie* Character = Cast<ACharacterZombie>(Owner))
		{
			Character->OnAttackEnd();
		}
	}
}
