// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIState.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

class UAISettings;
UINTERFACE(BlueprintType)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TKOU_GAMEPLAYTEST_API IAIInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetState(EAIState NewState);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EAIState GetState() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetState(EAIState state);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAISettings* GetSettings() const;
};
