#include "Gameplay/Climb/ClimbTrigger.h"

#include "GameFramework/Character.h"
#include "Interfaces/GT_PlayerControllableInterface.h"
#include "Player/GT_PlayerController.h"
#include "Player/Components/ClimbComponent.h"

AClimbTrigger::AClimbTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	SetRootComponent(Trigger);

	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionObjectType(ECC_WorldDynamic);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);
}

void AClimbTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &AClimbTrigger::OnTriggerBeginOverlap);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &AClimbTrigger::OnTriggerEndOverlap);
	}
}

bool AClimbTrigger::CanClimb_Implementation()
{
	return true;
}

void AClimbTrigger::Climb_Implementation(ACharacter* character)
{
	if (!character || !AnimToPlay)
	{
		return;
	}

	if (character->GetClass()->ImplementsInterface(UGT_PlayerControllableInterface::StaticClass()))
	{
		IGT_PlayerControllableInterface::Execute_SetPlayerState(character, EGameplayPlayerState::Interaction);
	}

	character->SetActorLocation(Trigger->GetComponentLocation());
	if (AGT_PlayerController* Controller = Cast<AGT_PlayerController>(character->GetController()))
	{
		character->SetActorRotation(Trigger->GetComponentRotation(), ETeleportType::ResetPhysics);
		Controller->SetControlRotation(Trigger->GetComponentRotation());
		Controller->UpdateLookYawToCurrent();
	};
	
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, character]
	{
		character->PlayAnimMontage(AnimToPlay);
	});
}

float AClimbTrigger::GetDuration_Implementation()
{
	return AnimToPlay ? AnimToPlay->GetPlayLength() : 0.f;
}

void AClimbTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UClimbComponent* ClimbComponent = OtherActor ? OtherActor->FindComponentByClass<UClimbComponent>() : nullptr)
	{
		ClimbComponent->SetClimbTarget(this);
	}
}

void AClimbTrigger::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UClimbComponent* ClimbComponent = OtherActor ? OtherActor->FindComponentByClass<UClimbComponent>() : nullptr)
	{
		ClimbComponent->ClearClimbTarget(this);
	}
}

