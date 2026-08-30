#pragma once

#include "CoreMinimal.h"
#include "Companion/CompanionsSaveData.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSave.generated.h"

UCLASS(BlueprintType)
class TKOU_GAMEPLAYTEST_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Save", SaveGame)
	FString SaveSlotName = TEXT("Player");

	UPROPERTY(BlueprintReadWrite, Category = "Save", SaveGame)
	int32 UserIndex = 0;
	
	UPROPERTY(BlueprintReadWrite, Category = "Save", SaveGame)
	FCompanionsSaveData CompanionSaveData = FCompanionsSaveData();
};
