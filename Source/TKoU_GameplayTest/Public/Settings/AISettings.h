// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AISettings.generated.h"

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UAISettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float FollowPlayerRadius = 350;
};
