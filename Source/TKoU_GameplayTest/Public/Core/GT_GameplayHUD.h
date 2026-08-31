// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GT_GameplayHUD.generated.h"

class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_GameplayHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
