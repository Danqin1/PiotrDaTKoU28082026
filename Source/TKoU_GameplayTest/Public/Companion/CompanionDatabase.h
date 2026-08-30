// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompanionData.h"
#include "Engine/DataAsset.h"
#include "CompanionDatabase.generated.h"

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UCompanionDatabase : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UCompanionData*> DefaultCompanionsData;
	
public:
	UFUNCTION(BlueprintCallable)
	UCompanionData* GetCompanionData(FString ID)
	{
		if (DefaultCompanionsData.Contains(ID))
		{
			return DefaultCompanionsData[ID];
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Companion ID does not exist! = {%s}"), *ID)
			return nullptr;
		}
	}
};
