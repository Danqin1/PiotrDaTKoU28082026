// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIState.h"
#include "Player/GameplayPlayerState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DevDebugSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UDevDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	EAIState CompanionState;
	
	UPROPERTY(BlueprintReadWrite)
	EGameplayPlayerState PlayerState;
};
