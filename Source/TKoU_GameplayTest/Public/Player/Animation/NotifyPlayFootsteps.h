// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NotifyPlayFootsteps.generated.h"

class UPhysicalMaterial;
class USoundBase;

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UNotifyPlayFootsteps : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Footsteps|Physical Materials")
	UPhysicalMaterial* DirtMat;
	
	UPROPERTY(EditAnywhere, Category = "Footsteps|Physical Materials")
	UPhysicalMaterial* FloorMat;

	UPROPERTY(EditAnywhere, Category = "Footsteps|Sounds")
	USoundBase* DirtSound;

	UPROPERTY(EditAnywhere, Category = "Footsteps|Sounds")
	USoundBase* FloorSound;

	UPROPERTY(EditAnywhere, Category = "Footsteps|Sounds")
	USoundBase* DefaultSound;

	UPROPERTY(EditAnywhere, Category = "Footsteps|Trace", meta = (ClampMin = "0.0"))
	float TraceStartOffset = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Footsteps|Trace", meta = (ClampMin = "0.0"))
	float TraceDistance = 200.0f;
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
