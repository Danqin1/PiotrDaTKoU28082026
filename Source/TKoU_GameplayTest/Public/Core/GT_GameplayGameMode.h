// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GT_GameplayGameMode.generated.h"

class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_GameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
