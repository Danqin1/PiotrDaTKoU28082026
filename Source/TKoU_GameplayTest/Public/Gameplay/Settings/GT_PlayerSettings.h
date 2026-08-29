#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GT_PlayerSettings.generated.h"

UCLASS(BlueprintType)
class TKOU_GAMEPLAYTEST_API UGT_PlayerSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float SprintSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float FacingRotationInterpSpeed = 12.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look", meta = (Units = "Degrees"))
	FVector2D TopDownLookHorizontalLimits = FVector2D(-180.f, 180.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look", meta = (Units = "Degrees"))
	FVector2D TopDownLookVerticalLimits = FVector2D(-80.f, -35.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look", meta = (ClampMin = "0.0", Units = "DegreesPerSecond"))
	float HorizontalLookRotationSpeed = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look", meta = (ClampMin = "0.0", Units = "DegreesPerSecond"))
	float VerticalLookRotationSpeed = 90.f;
};
