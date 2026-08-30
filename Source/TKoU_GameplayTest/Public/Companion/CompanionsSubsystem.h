// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompanionDatabase.h"
#include "Core/GT_GameInstanceSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CompanionsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UCompanionsSubsystem : public UGT_GameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
	UPlayerSave* PlayerSave;
	
	UPROPERTY(Transient)
	UCompanionDatabase* CompanionsDatabase;
	
	UPROPERTY(Transient)
	ACompanion* ActiveCompanion;

public:
	
	virtual void PostInit() override;
	virtual void Restore(UPlayerSave* Save) override;
	
	UFUNCTION(BlueprintCallable)
	bool TrySpawnCompanion(AActor* Player);
	
	UFUNCTION(BlueprintCallable)
	ACompanion* GetCompanion();
};
