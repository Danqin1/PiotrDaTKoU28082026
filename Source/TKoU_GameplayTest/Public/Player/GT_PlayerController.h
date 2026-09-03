
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "GT_PlayerController.generated.h"


class UPlayerHUDWidget;
class UGT_PlayerSettings;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_PlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> FastRotateAction;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Look")
	float CurrentLookYaw = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Look")
	float CurrentLookPitch = 0.f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUDWidget> HudClass;
	
	UPROPERTY(Transient)
	UPlayerHUDWidget* HUD;
	
	bool bUsingGamepad;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	void Move(const FInputActionValue& Value);
	void StopMoving(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprinting();
	void StopSprinting();
	void FastRotate(const FInputActionValue& InputActionValue);

	virtual UPlayerHUDWidget* GetPlayerHUDWidget_Implementation() override;
	virtual bool InputKey(const FInputKeyEventArgs& Params) override;
private:

	void AddDefaultMappingContext();
	void RemoveDefaultMappingContext();
	void CacheCurrentLookRotation();
	void UpdateControlledCharacterViewRotation();
	void ValidateInputReferences() const;
	FRotator GetCameraYawRotation() const;
	float ClampLookAngle(float Angle, const FVector2D& Limits) const;
	UObject* GetControlledCharacter() const;
	
public:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void UpdateLookYawToCurrent();
};
