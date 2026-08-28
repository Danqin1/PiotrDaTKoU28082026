

#include "TKoU_GameplayTest/Public/Gameplay/Controllers/GT_PlayerController.h"

#include "TKoU_GameplayTest/Public/Gameplay/Characters/GT_MadelaineCharacter.h"

void AGT_PlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	
	if (InputComponent && CachedMadelaineCharacter)
	{
		const float ForwardValue =
			(IsInputKeyDown(EKeys::W) ? 1.f : 0.f) -
			(IsInputKeyDown(EKeys::S) ? 1.f : 0.f);

		const float TurnValue =
			(IsInputKeyDown(EKeys::D) ? 1.f : 0.f) -
			(IsInputKeyDown(EKeys::A) ? 1.f : 0.f);
		
		if (!FMath::IsNearlyZero(ForwardValue))
		{
			CachedMadelaineCharacter->AddForwardMovementInput(ForwardValue);
		}
		
		if (!FMath::IsNearlyZero(TurnValue))
		{
			const float TurnSpeedDegPerSec = 180.f;

			const FRotator NewRotation = CachedMadelaineCharacter->GetActorRotation() +
				FRotator(0.f, TurnValue * TurnSpeedDegPerSec * DeltaTime, 0.f);

			CachedMadelaineCharacter->SetActorRotation(NewRotation);
		}
	}
}

void AGT_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	CachedMadelaineCharacter = Cast<AGT_MadelaineCharacter>(InPawn);
}

void AGT_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	CachedMadelaineCharacter = nullptr;
}
