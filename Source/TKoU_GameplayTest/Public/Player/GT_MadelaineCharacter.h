// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ClimbComponent.h"
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

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCompanionComponent* CompanionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UClimbComponent* ClimbComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UGT_PlayerSettings> PlayerSettings;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input")
	FVector2D CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Look")
	float CurrentViewYaw = 0.f;
	
	virtual void AddPlayerMovementInput_Implementation(const FVector& WorldMovementInput, const FVector2D& CameraRelativeMovementInput) override;
	virtual void StopPlayerMovementInput_Implementation() override;
	virtual void SetPlayerViewRotation_Implementation(const FRotator& ViewRotation) override;
	virtual void SetSprinting_Implementation(bool bShouldSprint) override;

	virtual void SetPlayerState_Implementation(EGameplayPlayerState state) override;
	virtual EGameplayPlayerState GetPlayerState_Implementation() override;
	virtual void ResetStateFrom_Implementation(EGameplayPlayerState stateFromReset) override;
	virtual bool IsSprinting_Implementation() override;
	virtual FVector2D GetCurrentCameraRelativeMovementInput_Implementation() const override;
	virtual FVector GetPlayerVelocity_Implementation() override;
	virtual float GetMaxSpeed_Implementation() override;
	
	void ApplyMovementSpeed() const;
	void ApplyMovementRotationSettings() const;
	void UpdateFacingFromVelocity();
	void FaceCurrentLookDirection();
	void FaceDirection(const FVector& Direction);
};
