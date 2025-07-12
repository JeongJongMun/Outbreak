#include "MeshLoadHelper.h"
#include "StringFormatHelper.h"

TObjectPtr<USkeletalMesh> MeshLoadHelper::GetRandomZombieMesh(const FString& MeshType, const int32 MeshCount)
{
	const FString BaseMeshRef = TEXT("/Script/Engine.SkeletalMesh'/Game/Art/Characters/Zombies/Meshes");
	const FString BaseMeshAssetString = TEXT("SKM_Zombie");
	
	const int32 MeshIndex = FMath::RandRange(1, MeshCount);
	const FString MeshIndexString = StringFormatHelper::ToPaddedString(MeshIndex, 3);

	// ex : SKM_Zombie_Normal_001
	const FString MeshAssetName = FString::Printf(TEXT("%s_%s_%s"), *BaseMeshAssetString, *MeshType, *MeshIndexString);
	// ex : /Script/Engine.SkeletalMesh'/Game/Art/Characters/Zombies/Meshes/SKM_Zombie_Fat_002.SKM_Zombie_Fat_002'
	const FString MeshPath = FString::Printf(TEXT("%s/%s.%s'"), *BaseMeshRef, *MeshAssetName, *MeshAssetName);
	
	TObjectPtr<USkeletalMesh> ZombieMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *MeshPath));
	if (!ZombieMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load Zombie mesh: %s"), CURRENT_CONTEXT, *MeshPath);
		return nullptr;
	}
	
	return ZombieMesh;
}