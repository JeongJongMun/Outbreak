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
	virtual void InitializeZombieData(FZombieData* InData);
	virtual void OnAttackEnd();
	FZombieData* GetZombieData() { return &ZombieData; }
	void PlayAnimation(EZombieStateType AnimType);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Die() override;
	virtual void SetMesh(EZombieMeshType MeshType);
	void ChangeZombieState(EZombieStateType NewState, TObjectPtr<ACharacterPlayer> TargetPlayer = nullptr);

// --------------------
// Variables
// --------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	FZombieData ZombieData;
	
	TMap<EZombieStateType, FName> MontageSectionNameMap;
	FName CurrentSection = "Idle";
	FName IdleSectionName = "Idle";
	FName WanderSectionName = "Wander";
	FName ChaseWalkSectionName = "ChaseWalk";
	FName ChaseRunSectionName = "ChaseRun";
	FName AttackSectionName = "Attack";
	FName DieSectionName = "Die";

	FString BaseMeshRef = TEXT("/Script/Engine.SkeletalMesh'/Game/Meshes/Zombie");
	FString BaseMeshName = TEXT("SKM_Zombie");
	int NormalMeshCount = 20;
	int MuscleMeshCount = 6;
	int FatMeshCount = 6;
	
private:
	TObjectPtr<AZombieAI> ZombieAIController;
	TObjectPtr<UAnimMontage> AnimMontage;
};