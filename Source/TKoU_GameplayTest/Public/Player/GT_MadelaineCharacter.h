// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CompanionComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/GT_PlayerControllableInterface.h"
#include "GT_MadelaineCharacter.generated.h"


class UGT_PlayerSettings;

UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_MadelaineCharacter : public ACharacter, public IGT_PlayerControllableInterface
{
	GENERATED_BODY()

public:

	AGT_MadelaineCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void AddForwardMovementInput(float Value);

	virtual void AddPlayerMovementInput_Implementation(const FVector& WorldMovementInput, const FVector2D& CameraRelativeMovementInput) override;
	virtual void StopPlayerMovementInput_Implementation() override;
	virtual void SetPlayerViewRotation_Implementation(const FRotator& ViewRotation) override;
	virtual void SetSprinting_Implementation(bool bShouldSprint) override;

	UFUNCTION(BlueprintPure, Category = "Input")
	FVector2D GetCurrentCameraRelativeMovementInput() const;

	UFUNCTION(BlueprintPure, Category = "Input")
	FVector2D GetDirectionRelativeToCamera(const FVector& WorldDirection) const;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCompanionComponent* CompanionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UGT_PlayerSettings> PlayerSettings;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input")
	FVector2D CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Look")
	float CurrentViewYaw = 0.f;

	void ApplyMovementSpeed() const;
	void ApplyMovementRotationSettings() const;
	void UpdateFacingFromVelocity();
	void FaceCurrentLookDirection();
	void FaceDirection(const FVector& Direction);
};
