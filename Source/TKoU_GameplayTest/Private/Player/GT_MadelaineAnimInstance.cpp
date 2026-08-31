#include "Player/GT_MadelaineAnimInstance.h"
#include "Settings/GT_PlayerSettings.h"

void UGT_MadelaineAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Owner = TryGetPawnOwner();
}

void UGT_MadelaineAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Owner && Settings)
	{
		PlayerVelocity = IGT_PlayerControllableInterface::Execute_GetPlayerVelocity(Owner);
		CurrentCameraRelativeMovementInput = IGT_PlayerControllableInterface::Execute_GetCurrentCameraRelativeMovementInput(Owner);
		bool bIsSprinting = IGT_PlayerControllableInterface::Execute_IsSprinting(Owner);
		float maxSpeed = IGT_PlayerControllableInterface::Execute_GetMaxSpeed(Owner);
		float lerp = bIsSprinting ? Settings->AnimLerpSpeed / 5 * DeltaSeconds : Settings->AnimLerpSpeed * DeltaSeconds;
		
		if (bIsSprinting)
		{
			float X = FMath::Lerp(AnimVelocity.X, 0, lerp);
			float Y = FMath::Lerp(AnimVelocity.Y, CurrentCameraRelativeMovementInput.Length() * maxSpeed, lerp);
			
			AnimVelocity = FVector2D(X, Y);
		}
		else
		{
			float speed = PlayerVelocity.Length();
			FVector2D dir = CurrentCameraRelativeMovementInput * speed;
			
			float X = FMath::Lerp(AnimVelocity.X, dir.Y, lerp);
			float Y = FMath::Lerp(AnimVelocity.Y, dir.X, lerp);
			
			AnimVelocity = FVector2D(X, Y);
		}
		
	}
	Super::NativeUpdateAnimation(DeltaSeconds);
}
