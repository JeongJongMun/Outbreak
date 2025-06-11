// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Outbreak/Character/CharacterBase.h"
#include "Outbreak/Character/Player/CharacterPlayer.h"
#include "Outbreak/Util/Define.h"
#include "State/ZombieAI.h"
#include "CharacterZombie.generated.h"

UCLASS()
class OUTBREAK_API ACharacterZombie : public ACharacterBase
{
	GENERATED_BODY()

// --------------------
// Functions
// --------------------
public:
	ACharacterZombie();
	virtual void OnAttackEnd();
	FZombieData* GetZombieData() { return &ZombieData; }
	void PlayAnimation(EZombieStateType InStateType);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitCharacterData() override;
	virtual void SetupCollision() override;
	virtual void SetupMovement() override;
	virtual void Die() override;
	virtual void SetMesh(ECharacterBodyType MeshType);
	void ChangeZombieState(EZombieStateType NewState, TObjectPtr<ACharacterPlayer> TargetPlayer = nullptr);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,class AActor* DamageCauser) override;

private:
	EZombieAnimationType GetZombieAnimationTypeFromState(EZombieStateType InStateType);
	
// --------------------
// Variables
// --------------------
protected:
	EZombieType ZombieType = EZombieType::None;
	EZombieSubType ZombieSubType = EZombieSubType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	FZombieData ZombieData;

	UPROPERTY()
	AController* LastDamagePlayer;

	UPROPERTY()
	TMap<EZombieAnimationType, TObjectPtr<UAnimMontage>> AnimMontageMap;
	TMap<EZombieAnimationType, TArray<FName>> AnimSectionMap;
	
	FString BaseMeshRef = TEXT("/Script/Engine.SkeletalMesh'/Game/Meshes/Character");
	FString BaseMeshName = TEXT("SKM_Zombie");
	int NormalMeshCount = 20;
	int MuscleMeshCount = 6;
	int FatMeshCount = 6;
	
private:
	TObjectPtr<AZombieAI> ZombieAI;
};