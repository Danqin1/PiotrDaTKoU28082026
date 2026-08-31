#include "Player/Components/ClimbComponent.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/ClimbableInterface.h"
#include "Interfaces/GT_PlayerControllableInterface.h"

UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	BindInput();
}

void UClimbComponent::SetClimbTarget(AActor* NewClimbTarget)
{
	if (!NewClimbTarget || !NewClimbTarget->GetClass()->ImplementsInterface(UClimbableInterface::StaticClass()))
	{
		return;
	}

	CurrentClimbTarget = NewClimbTarget;
	bCanClimb = IClimbableInterface::Execute_CanClimb(NewClimbTarget);
}

void UClimbComponent::ClearClimbTarget(AActor* ClimbTargetToClear)
{
	if (CurrentClimbTarget != ClimbTargetToClear)
	{
		return;
	}

	CurrentClimbTarget = nullptr;
	bCanClimb = false;
}

void UClimbComponent::OnClimbInput(const FInputActionValue& InputActionValue)
{
	if (!bCanClimb || !OwnerCharacter || !CurrentClimbTarget)
	{
		return;
	}

	if (!CurrentClimbTarget->GetClass()->ImplementsInterface(UClimbableInterface::StaticClass()))
	{
		ClearClimbTarget(CurrentClimbTarget);
		return;
	}

	if (OwnerCharacter->GetClass()->ImplementsInterface(UGT_PlayerControllableInterface::StaticClass()) &&
		IGT_PlayerControllableInterface::Execute_GetPlayerState(OwnerCharacter) != EGameplayPlayerState::Nothing)
	{
		return;
	}

	PrepareCharacterForClimb();
	IClimbableInterface::Execute_Climb(CurrentClimbTarget, OwnerCharacter);

	const float ClimbDuration = IClimbableInterface::Execute_GetDuration(CurrentClimbTarget);
	if (UWorld* World = GetWorld(); ClimbDuration > 0.f && World)
	{
		World->GetTimerManager().SetTimer(ClimbFinishTimerHandle, this, &UClimbComponent::FinishClimb,
			ClimbDuration, false);
	}
	else
	{
		FinishClimb();
	}
}

void UClimbComponent::BindInput()
{
	if (!ClimbAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing ClimbAction input reference."), *GetName());
		return;
	}

	if (UEnhancedInputComponent* Input = GetOwner()->FindComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(ClimbAction, ETriggerEvent::Started, this, &UClimbComponent::OnClimbInput);
		return;
	}

	if (const APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController()))
		{
			if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				Input->BindAction(ClimbAction, ETriggerEvent::Started, this, &UClimbComponent::OnClimbInput);
			}
		}
	}
}

void UClimbComponent::PrepareCharacterForClimb()
{
	if (!OwnerCharacter)
	{
		return;
	}
	
	IGT_PlayerControllableInterface::Execute_SetSprinting(OwnerCharacter, false);

	if (UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		if (!bHasSavedMovementMode)
		{
			PreviousMovementMode = MovementComponent->MovementMode;
			PreviousCustomMovementMode = MovementComponent->CustomMovementMode;
			bHasSavedMovementMode = true;
		}

		MovementComponent->StopMovementImmediately();
		MovementComponent->SetMovementMode(MOVE_Flying);
	}
}

void UClimbComponent::FinishClimb()
{
	if (OwnerCharacter)
	{
		if (UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			if (bHasSavedMovementMode)
			{
				MovementComponent->SetMovementMode(PreviousMovementMode.GetValue(), PreviousCustomMovementMode);
				bHasSavedMovementMode = false;
			}
		}
	}

	if (OwnerCharacter && OwnerCharacter->GetClass()->ImplementsInterface(UGT_PlayerControllableInterface::StaticClass()))
	{
		IGT_PlayerControllableInterface::Execute_ResetStateFrom(OwnerCharacter, EGameplayPlayerState::Interaction);
	}
}
