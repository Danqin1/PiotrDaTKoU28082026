// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NavInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UNavInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TKOU_GAMEPLAYTEST_API INavInterface
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartNavLinkAction(FVector start, FVector end);
};
