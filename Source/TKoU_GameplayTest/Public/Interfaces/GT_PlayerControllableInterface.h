#pragma once

#include "CoreMinimal.h"
#include "Player/GameplayPlayerState.h"
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

protected:
	EGameplayPlayerState State = EGameplayPlayerState::Nothing;
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddPlayerMovementInput(const FVector& WorldMovementInput, const FVector2D& CameraRelativeMovementInput);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopPlayerMovementInput();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPlayerViewRotation(const FRotator& ViewRotation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSprinting(bool bShouldSprint);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPlayerState(EGameplayPlayerState state);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EGameplayPlayerState GetPlayerState();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetStateFrom(EGameplayPlayerState stateFromReset);
};
