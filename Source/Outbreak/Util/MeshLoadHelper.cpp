#include "MeshLoadHelper.h"
#include "StringFormatHelper.h"

TObjectPtr<USkeletalMesh> MeshLoadHelper::GetRandomZombieMesh(const FString& BaseMeshRef, const FString& BaseMeshAssetNameRef,
                                                          const FString& MeshType, const int32 MeshCount)
{
	const int32 MeshIndex = FMath::RandRange(1, MeshCount);
	const FString MeshIndexString = StringFormatHelper::ToPaddedString(MeshIndex, 3);

	// ex : SKM_Zombie_Normal_001
	const FString MeshAssetName = FString::Printf(TEXT("%s_%s_%s"), *BaseMeshAssetNameRef, *MeshType, *MeshIndexString);
	// ex : /Script/Engine.SkeletalMesh'/Game/Meshes/Zombie/Normal/SKM_Zombie_Normal_001.SKM_Zombie_Normal_001'
	const FString MeshPath = FString::Printf(TEXT("%s/%s.%s'"), *BaseMeshRef, *MeshAssetName, *MeshAssetName);
	
	TObjectPtr<USkeletalMesh> ZombieMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *MeshPath));
	if (!ZombieMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load Zombie mesh: %s"), CURRENT_CONTEXT, *MeshPath);
		return nullptr;
	}
	
	return ZombieMesh;
}

FString MeshLoadHelper::ZombieMeshTypeToString(const ECharacterBodyType ZombieMeshType)
{
	const UEnum* EnumPtr = StaticEnum<ECharacterBodyType>();
	if (!EnumPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to get ZombieMeshType enum"), CURRENT_CONTEXT);
		return FString();
	}
	
	return EnumPtr->GetNameStringByValue(static_cast<int64>(ZombieMeshType));
}
