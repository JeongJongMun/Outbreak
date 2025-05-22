// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Outbreak/Character/CharacterBase.h"
#include "Outbreak/Util/Define.h"
#include "CharacterZombie.generated.h"

UCLASS()
class OUTBREAK_API ACharacterZombie : public ACharacterBase
{
	GENERATED_BODY()

public:
	ACharacterZombie();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitializeZombieData(FZombieData* InData);
	FZombieData* GetZombieData() { return &ZombieData; }

	void PlayAnimation(EZombieAnimationType AnimType, bool bLoop = true)
	{
		if (AnimationMap.Contains(AnimType))
		{
			TObjectPtr<UAnimSequence> Anim = AnimationMap[AnimType];
			if (Anim)
			{
				GetMesh()->PlayAnimation(Anim, bLoop);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[%s] Animation is nullptr", CURRENT_CONTEXT));
			}
		}
	}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	FZombieData ZombieData;
	
	bool bIsAggressive;
	
private:
	TMap<EZombieAnimationType, TObjectPtr<UAnimSequence>> AnimationMap;
	TObjectPtr<UAnimSequence> IdleAnimation;
	TObjectPtr<UAnimSequence> WalkAnimation;
	TObjectPtr<UAnimSequence> RunAnimation;
	TObjectPtr<UAnimSequence> AttackAnimation;
};