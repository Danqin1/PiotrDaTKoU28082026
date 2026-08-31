// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GT_MadelaineCharacter.h"
#include "Animation/AnimInstance.h"
#include "GT_MadelaineAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UGT_MadelaineAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
	APawn* Owner;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D CurrentCameraRelativeMovementInput;
	
	UPROPERTY(BlueprintReadOnly)
	FVector PlayerVelocity;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D AnimVelocity;
	
	UPROPERTY(EditAnywhere)
	UGT_PlayerSettings* Settings;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
