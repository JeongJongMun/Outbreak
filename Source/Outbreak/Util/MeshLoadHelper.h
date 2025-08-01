﻿#pragma once
#include "Define.h"

class MeshLoadHelper
{
public:
	static TObjectPtr<USkeletalMesh> GetRandomZombieMesh(const FString& BaseMeshRef, const FString& BaseMeshAssetNameRef,
													  const FString& MeshType, const int32 MeshCount);
	static FString ZombieMeshTypeToString(const ECharacterBodyType ZombieMeshType);
};
