

#include "Player/GT_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/GT_PlayerControllableInterface.h"
#include "Save/SaveSubsystem.h"
#include "Settings/GT_PlayerSettings.h"
#include "UI/HUD/PlayerHUDWidget.h"

void AGT_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ValidateInputReferences();
	AddDefaultMappingContext();
	CacheCurrentLookRotation();
	
	if (IsLocalController())
	{
		HUD = CreateWidget<UPlayerHUDWidget>(this, HudClass);
		HUD->AddToViewport();
	}
	
}

void AGT_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveDefaultMappingContext();
	
	if (HUD)
	{
		HUD->RemoveFromViewport();
	}

	if (USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		SaveSubsystem->SavePlayerSave();
	}
	Super::EndPlay(EndPlayReason);
}

void AGT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, 
				&AGT_PlayerController::Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, 
				&AGT_PlayerController::StopMoving);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, 
				&AGT_PlayerController::StopMoving);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, 
				&AGT_PlayerController::Look);
		}

		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
				&AGT_PlayerController::StartSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
				&AGT_PlayerController::StopSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this,
				&AGT_PlayerController::StopSprinting);
		}
	}
}

void AGT_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CacheCurrentLookRotation();
	UpdateControlledCharacterViewRotation();
}

void AGT_PlayerController::AddDefaultMappingContext()
{
	if (!DefaultMappingContext)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGT_PlayerController::RemoveDefaultMappingContext()
{
	if (!DefaultMappingContext)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			InputSubsystem->RemoveMappingContext(DefaultMappingContext);
		}
	}
}

void AGT_PlayerController::Move(const FInputActionValue& Value)
{
	if (UObject* ControlledCharacter = GetControlledCharacter())
	{
		if (IGT_PlayerControllableInterface::Execute_GetPlayerState(ControlledCharacter) != EGameplayPlayerState::Nothing)
		{
			return;
		}
		const FVector2D MovementVector = Value.Get<FVector2D>();
		if (MovementVector.IsNearlyZero())
		{
			IGT_PlayerControllableInterface::Execute_StopPlayerMovementInput(ControlledCharacter);
			return;
		}

		UpdateControlledCharacterViewRotation();

		const FRotator CameraYawRotation = GetCameraYawRotation();
		const FVector CameraForward = CameraYawRotation.Vector();
		const FVector CameraRight = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::Y);
		const FVector2D CameraRelativeMovementInput =
			FVector2D(MovementVector.Y, MovementVector.X).GetClampedToMaxSize(1.f);
		const FVector WorldMovementInput =
			(CameraForward * MovementVector.Y + CameraRight * MovementVector.X).GetClampedToMaxSize(1.f);

		IGT_PlayerControllableInterface::Execute_AddPlayerMovementInput(ControlledCharacter, 
			WorldMovementInput, CameraRelativeMovementInput);
	}
}

void AGT_PlayerController::StopMoving(const FInputActionValue& Value)
{
	if (UObject* ControlledCharacter = GetControlledCharacter())
	{
		IGT_PlayerControllableInterface::Execute_StopPlayerMovementInput(ControlledCharacter);
	}
}

void AGT_PlayerController::Look(const FInputActionValue& Value)
{
	if (!PlayerSettings)
	{
		return;
	}

	const FVector2D LookVector = Value.Get<FVector2D>();
	if (LookVector.IsNearlyZero())
	{
		return;
	}

	const UWorld* World = GetWorld();
	const float DeltaSeconds = World ? World->GetDeltaSeconds() : 0.f;

	CurrentLookYaw += LookVector.X * PlayerSettings->HorizontalLookRotationSpeed * DeltaSeconds;
	CurrentLookPitch += LookVector.Y * PlayerSettings->VerticalLookRotationSpeed * DeltaSeconds;

	CurrentLookYaw = ClampLookAngle(CurrentLookYaw, PlayerSettings->TopDownLookHorizontalLimits);
	CurrentLookPitch = ClampLookAngle(CurrentLookPitch, PlayerSettings->TopDownLookVerticalLimits);

	SetControlRotation(FRotator(CurrentLookPitch, CurrentLookYaw, 0.f));
	UpdateControlledCharacterViewRotation();
}

void AGT_PlayerController::StartSprinting()
{
	if (UObject* ControlledCharacter = GetControlledCharacter())
	{
		if (IGT_PlayerControllableInterface::Execute_GetPlayerState(ControlledCharacter) != EGameplayPlayerState::Nothing)
		{
			return;
		}
		IGT_PlayerControllableInterface::Execute_SetSprinting(ControlledCharacter, true);
	}
}

void AGT_PlayerController::StopSprinting()
{
	if (UObject* ControlledCharacter = GetControlledCharacter())
	{
		IGT_PlayerControllableInterface::Execute_SetSprinting(ControlledCharacter, false);
	}
}

void AGT_PlayerController::CacheCurrentLookRotation()
{
	const FRotator CurrentControlRotation = GetControlRotation();
	CurrentLookYaw = FMath::UnwindDegrees(CurrentControlRotation.Yaw);
	CurrentLookPitch = FMath::UnwindDegrees(CurrentControlRotation.Pitch);

	if (PlayerSettings)
	{
		CurrentLookYaw = ClampLookAngle(CurrentLookYaw, PlayerSettings->TopDownLookHorizontalLimits);
		CurrentLookPitch = ClampLookAngle(CurrentLookPitch, PlayerSettings->TopDownLookVerticalLimits);
	}

	SetControlRotation(FRotator(CurrentLookPitch, CurrentLookYaw, 0.f));
	UpdateControlledCharacterViewRotation();
}

void AGT_PlayerController::ValidateInputReferences() const
{
	if (!PlayerSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing PlayerSettings reference."), *GetName());
	}

	if (!DefaultMappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing DefaultMappingContext input reference."), *GetName());
	}

	if (!MoveAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing MoveAction input reference."), *GetName());
	}

	if (!LookAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing LookAction input reference."), *GetName());
	}

	if (!SprintAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing SprintAction input reference."), *GetName());
	}
}

float AGT_PlayerController::ClampLookAngle(float Angle, const FVector2D& Limits) const
{
	const float MinAngle = FMath::Min(Limits.X, Limits.Y);
	const float MaxAngle = FMath::Max(Limits.X, Limits.Y);

	return FMath::Clamp(FMath::UnwindDegrees(Angle), MinAngle, MaxAngle);
}

UObject* AGT_PlayerController::GetControlledCharacter() const
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || !ControlledPawn->GetClass()->ImplementsInterface(UGT_PlayerControllableInterface::StaticClass()))
	{
		return nullptr;
	}

	return ControlledPawn;
}

void AGT_PlayerController::UpdateControlledCharacterViewRotation()
{
	if (UObject* ControlledCharacter = GetControlledCharacter())
	{
		FVector ViewLocation;
		FRotator ViewRotation;
		GetPlayerViewPoint(ViewLocation, ViewRotation);
		IGT_PlayerControllableInterface::Execute_SetPlayerViewRotation(ControlledCharacter, ViewRotation);
	}
}

FRotator AGT_PlayerController::GetCameraYawRotation() const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewLocation, ViewRotation);

	return FRotator(0.f, ViewRotation.Yaw, 0.f);
}
