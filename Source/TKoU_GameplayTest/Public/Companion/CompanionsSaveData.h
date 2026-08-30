#pragma once

#include "CoreMinimal.h"
#include "CompanionsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FCompanionsSaveData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> UnlockedCompanions = {"Scooby"};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChosenCompanion = "Scooby"; // we dont have any UI to decide yet
};
