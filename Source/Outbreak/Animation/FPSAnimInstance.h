#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPSAnimInstance.generated.h"

UCLASS()
class OUTBREAK_API UFPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 업데이트 메서드
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 애니메이션에서 읽을 변수들
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsCrouching;
};
