// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimEnums.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "ClimbComponent.generated.h"

class ACharacter;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TKOU_GAMEPLAYTEST_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UClimbComponent();

	void SetClimbTarget(AActor* NewClimbTarget);
	void ClearClimbTarget(AActor* ClimbTargetToClear);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ClimbAction;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Climb")
	bool bCanClimb = false;

	UPROPERTY(Transient)
	AActor* CurrentClimbTarget = nullptr;

	UPROPERTY(Transient)
	ACharacter* OwnerCharacter = nullptr;

	FTimerHandle ClimbFinishTimerHandle;
	TEnumAsByte<EMovementMode> PreviousMovementMode = MOVE_None;
	uint8 PreviousCustomMovementMode = 0;
	bool bHasSavedMovementMode = false;

	UFUNCTION()
	void OnClimbInput(const FInputActionValue& InputActionValue);

	void BindInput();
	void PrepareCharacterForClimb();
	void FinishClimb();
};
