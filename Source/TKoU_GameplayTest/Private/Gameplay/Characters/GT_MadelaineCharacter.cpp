
#include "TKoU_GameplayTest/Public/Gameplay/Characters/GT_MadelaineCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "TKoU_GameplayTest/Public/Gameplay/Settings/GT_PlayerSettings.h"

AGT_MadelaineCharacter::AGT_MadelaineCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	ApplyMovementRotationSettings();
	ApplyMovementSpeed();
}

void AGT_MadelaineCharacter::BeginPlay()
{
	Super::BeginPlay();

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

	CacheCurrentLookRotation();
	ApplyMovementRotationSettings();
	ApplyMovementSpeed();
}

void AGT_MadelaineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFacingFromVelocity();
}

void AGT_MadelaineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CacheCurrentLookRotation();

	if (DefaultMappingContext)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
					ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGT_MadelaineCharacter::Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGT_MadelaineCharacter::StopMoving);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AGT_MadelaineCharacter::StopMoving);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGT_MadelaineCharacter::Look);
		}

		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGT_MadelaineCharacter::StartSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGT_MadelaineCharacter::StopSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AGT_MadelaineCharacter::StopSprinting);
		}
	}
}

void AGT_MadelaineCharacter::AddForwardMovementInput(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

FVector2D AGT_MadelaineCharacter::GetCurrentCameraRelativeMovementInput() const
{
	return CurrentCameraRelativeMovementInput;
}

FVector2D AGT_MadelaineCharacter::GetDirectionRelativeToCamera(const FVector& WorldDirection) const
{
	const FVector FlatDirection = FVector(WorldDirection.X, WorldDirection.Y, 0.f).GetSafeNormal();
	if (FlatDirection.IsNearlyZero())
	{
		return FVector2D::ZeroVector;
	}

	const FRotator CameraYawRotation(0.f, CurrentLookYaw, 0.f);
	const FVector CameraForward = CameraYawRotation.Vector();
	const FVector CameraRight = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::Y);

	return FVector2D(
		FVector::DotProduct(FlatDirection, CameraForward),
		FVector::DotProduct(FlatDirection, CameraRight));
}

void AGT_MadelaineCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.IsNearlyZero())
	{
		CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;
		return;
	}

	CurrentCameraRelativeMovementInput = FVector2D(MovementVector.Y, MovementVector.X).GetClampedToMaxSize(1.f);

	const FRotator CameraYawRotation(0.f, CurrentLookYaw, 0.f);
	const FVector CameraForward = CameraYawRotation.Vector();
	const FVector CameraRight = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::Y);

	if (!FMath::IsNearlyZero(MovementVector.Y))
	{
		AddMovementInput(CameraForward, MovementVector.Y);
	}

	if (!FMath::IsNearlyZero(MovementVector.X))
	{
		AddMovementInput(CameraRight, MovementVector.X);
	}
}

void AGT_MadelaineCharacter::StopMoving(const FInputActionValue& Value)
{
	CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;
}

void AGT_MadelaineCharacter::Look(const FInputActionValue& Value)
{
	if (!PlayerSettings || !Controller)
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

	Controller->SetControlRotation(FRotator(CurrentLookPitch, CurrentLookYaw, 0.f));
}

void AGT_MadelaineCharacter::StartSprinting()
{
	bIsSprinting = true;
	ApplyMovementSpeed();
}

void AGT_MadelaineCharacter::StopSprinting()
{
	bIsSprinting = false;
	ApplyMovementSpeed();
}

void AGT_MadelaineCharacter::ApplyMovementSpeed() const
{
	if (!PlayerSettings)
	{
		return;
	}

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = bIsSprinting ? PlayerSettings->SprintSpeed : PlayerSettings->WalkSpeed;
	}
}

void AGT_MadelaineCharacter::ApplyMovementRotationSettings() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->bOrientRotationToMovement = false;
		MovementComponent->bUseControllerDesiredRotation = false;
	}
}

void AGT_MadelaineCharacter::CacheCurrentLookRotation()
{
	if (!Controller)
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	CurrentLookYaw = FMath::UnwindDegrees(ControlRotation.Yaw);
	CurrentLookPitch = FMath::UnwindDegrees(ControlRotation.Pitch);

	if (PlayerSettings)
	{
		CurrentLookYaw = ClampLookAngle(CurrentLookYaw, PlayerSettings->TopDownLookHorizontalLimits);
		CurrentLookPitch = ClampLookAngle(CurrentLookPitch, PlayerSettings->TopDownLookVerticalLimits);
	}
}

void AGT_MadelaineCharacter::UpdateFacingFromVelocity()
{
	const FVector Velocity = GetVelocity();
	if (Velocity.SizeSquared2D() <= UE_KINDA_SMALL_NUMBER)
	{
		return;
	}

	if (bIsSprinting)
	{
		FaceDirection(Velocity);
	}
	else
	{
		FaceCurrentLookDirection();
	}
}

void AGT_MadelaineCharacter::FaceCurrentLookDirection()
{
	if (!PlayerSettings)
	{
		return;
	}

	const FRotator TargetRotation(0.f, CurrentLookYaw, 0.f);
	const UWorld* World = GetWorld();
	const float DeltaSeconds = World ? World->GetDeltaSeconds() : 0.f;

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, PlayerSettings->FacingRotationInterpSpeed));
}

void AGT_MadelaineCharacter::FaceDirection(const FVector& Direction)
{
	if (!PlayerSettings)
	{
		return;
	}

	const FVector FlatDirection = FVector(Direction.X, Direction.Y, 0.f);
	if (FlatDirection.IsNearlyZero())
	{
		return;
	}

	const FRotator TargetRotation = FlatDirection.Rotation();
	const UWorld* World = GetWorld();
	const float DeltaSeconds = World ? World->GetDeltaSeconds() : 0.f;

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, PlayerSettings->FacingRotationInterpSpeed));
}

float AGT_MadelaineCharacter::ClampLookAngle(float Angle, const FVector2D& Limits) const
{
	const float MinAngle = FMath::Min(Limits.X, Limits.Y);
	const float MaxAngle = FMath::Max(Limits.X, Limits.Y);

	return FMath::Clamp(FMath::UnwindDegrees(Angle), MinAngle, MaxAngle);
}
