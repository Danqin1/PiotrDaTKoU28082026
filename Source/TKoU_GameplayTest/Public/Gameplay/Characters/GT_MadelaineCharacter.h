// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GT_MadelaineCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
class UGT_PlayerSettings;
struct FInputActionValue;

UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_MadelaineCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AGT_MadelaineCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AddForwardMovementInput(float Value);

	UFUNCTION(BlueprintPure, Category = "Input")
	FVector2D GetCurrentCameraRelativeMovementInput() const;

	UFUNCTION(BlueprintPure, Category = "Input")
	FVector2D GetDirectionRelativeToCamera(const FVector& WorldDirection) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UGT_PlayerSettings> PlayerSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input")
	FVector2D CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Look")
	float CurrentLookYaw = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Look")
	float CurrentLookPitch = 0.f;

	void Move(const FInputActionValue& Value);
	void StopMoving(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprinting();
	void StopSprinting();
	void ApplyMovementSpeed() const;
	void ApplyMovementRotationSettings() const;
	void CacheCurrentLookRotation();
	void UpdateFacingFromVelocity();
	void FaceCurrentLookDirection();
	void FaceDirection(const FVector& Direction);
	float ClampLookAngle(float Angle, const FVector2D& Limits) const;
};
