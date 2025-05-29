// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ZombieAttackEnd.h"

#include "Outbreak/Character/Zombie/CharacterZombie.h"

void UAnimNotify_ZombieAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ACharacterZombie* Character = Cast<ACharacterZombie>(Owner))
		{
			Character->OnAttackEnd();
			UE_LOG(LogTemp, Log, TEXT("[%s] Notify: Zombie attack ended."), CURRENT_CONTEXT);
		}
	}
}
