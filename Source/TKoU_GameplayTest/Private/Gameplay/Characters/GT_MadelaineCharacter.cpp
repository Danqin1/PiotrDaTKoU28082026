
#include "TKoU_GameplayTest/Public/Gameplay/Characters/GT_MadelaineCharacter.h"


AGT_MadelaineCharacter::AGT_MadelaineCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGT_MadelaineCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGT_MadelaineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGT_MadelaineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGT_MadelaineCharacter::AddForwardMovementInput(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}