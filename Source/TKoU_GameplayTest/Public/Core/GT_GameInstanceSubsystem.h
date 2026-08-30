// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GT_GameInstanceSubsystem.generated.h"

class UPlayerSave;
/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UGT_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void PostInit();
	virtual void Restore(UPlayerSave* Save);
};
