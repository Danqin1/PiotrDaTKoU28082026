#pragma once
#include "CompanionData.generated.h"

class ACompanion;

UENUM(BlueprintType)
enum class ECompanionType : uint8
{
	Dog,
	Cat,
	Dragon
};


UCLASS(BlueprintType)
class UCompanionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECompanionType Type = ECompanionType::Dog;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CompanionName = "My Companion";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP = 100;
	
	float XP = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACompanion> Blueprint;
};
