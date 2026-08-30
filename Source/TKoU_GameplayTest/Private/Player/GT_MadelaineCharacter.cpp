
#include "Player/GT_MadelaineCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/GT_PlayerSettings.h"

AGT_MadelaineCharacter::AGT_MadelaineCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	ApplyMovementRotationSettings();
	ApplyMovementSpeed();
	
	CompanionComp = CreateDefaultSubobject<UCompanionComponent>(TEXT("CompanionComp"));
}

void AGT_MadelaineCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing PlayerSettings reference."), *GetName());
	}

	ApplyMovementRotationSettings();
	ApplyMovementSpeed();
}

void AGT_MadelaineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFacingFromVelocity();
}

void AGT_MadelaineCharacter::AddForwardMovementInput(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AGT_MadelaineCharacter::AddPlayerMovementInput_Implementation(
	const FVector& WorldMovementInput,
	const FVector2D& CameraRelativeMovementInput)
{
	if (WorldMovementInput.IsNearlyZero())
	{
		CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;
		return;
	}

	CurrentCameraRelativeMovementInput = CameraRelativeMovementInput.GetClampedToMaxSize(1.f);
	AddMovementInput(WorldMovementInput.GetSafeNormal2D(), FMath::Min(WorldMovementInput.Size2D(), 1.f));
}

void AGT_MadelaineCharacter::StopPlayerMovementInput_Implementation()
{
	CurrentCameraRelativeMovementInput = FVector2D::ZeroVector;
}

void AGT_MadelaineCharacter::SetPlayerViewRotation_Implementation(const FRotator& ViewRotation)
{
	CurrentViewYaw = ViewRotation.Yaw;
}

void AGT_MadelaineCharacter::SetSprinting_Implementation(bool bShouldSprint)
{
	bIsSprinting = bShouldSprint;
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

FVector2D AGT_MadelaineCharacter::GetCurrentCameraRelativeMovementInput() const
{
	return CurrentCameraRelativeMovementInput;
}

FVector2D AGT_MadelaineCharacter::GetDirectionRelativeToCamera(const FVector& WorldDirection) const
{
	if (!CurrentCameraRelativeMovementInput.IsNearlyZero())
	{
		return CurrentCameraRelativeMovementInput;
	}

	const FVector FlatDirection = FVector(WorldDirection.X, WorldDirection.Y, 0.f).GetSafeNormal();
	if (FlatDirection.IsNearlyZero())
	{
		return FVector2D::ZeroVector;
	}

	const FRotator CameraYawRotation(0.f, CurrentViewYaw, 0.f);
	const FVector CameraForward = CameraYawRotation.Vector();
	const FVector CameraRight = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::Y);

	return FVector2D(
		FVector::DotProduct(FlatDirection, CameraForward),
		FVector::DotProduct(FlatDirection, CameraRight));
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

	const FRotator TargetRotation(0.f, CurrentViewYaw, 0.f);
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
