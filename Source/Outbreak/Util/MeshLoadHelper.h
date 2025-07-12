#pragma once
#include "Define.h"

class MeshLoadHelper
{
public:
	static TObjectPtr<USkeletalMesh> GetRandomZombieMesh(const FString& MeshType, const int32 MeshCount);
	
};
