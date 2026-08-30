#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GT_PlayerControllableInterface.generated.h"

UINTERFACE(BlueprintType)
class TKOU_GAMEPLAYTEST_API UGT_PlayerControllableInterface : public UInterface
{
	GENERATED_BODY()
};

class TKOU_GAMEPLAYTEST_API IGT_PlayerControllableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character|Input")
	void AddPlayerMovementInput(const FVector& WorldMovementInput, const FVector2D& CameraRelativeMovementInput);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character|Input")
	void StopPlayerMovementInput();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character|Input")
	void SetPlayerViewRotation(const FRotator& ViewRotation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character|Movement")
	void SetSprinting(bool bShouldSprint);
};
