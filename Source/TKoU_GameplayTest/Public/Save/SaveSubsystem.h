// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GT_GameInstanceSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

class UPlayerSave;

UCLASS()
class TKOU_GAMEPLAYTEST_API USaveSubsystem : public UGT_GameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void PostInit() override;

	UFUNCTION(BlueprintCallable, Category = "Save")
	UPlayerSave* CreateNewPlayerSave(const FString& SlotName, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Save")
	UPlayerSave* LoadPlayerSave(const FString& SlotName, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SavePlayerSave();
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool DeletePlayerSave(const FString& SlotName, int32 UserIndex = 0);

	UFUNCTION(BlueprintPure, Category = "Save")
	bool DoesPlayerSaveExist(const FString& SlotName, int32 UserIndex = 0) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save")
	FString DefaultPlayerSaveSlotName = TEXT("Player");

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Save")
	TObjectPtr<UPlayerSave> CurrentPlayerSave;

private:

	static constexpr int32 DefaultUserIndex = 0;
};
