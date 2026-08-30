// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CompanionComponent.h"

#include "EnhancedInputComponent.h"
#include "AI/Companion/CompanionAIController.h"
#include "Companion/Companion.h"
#include "Companion/CompanionsSubsystem.h"

UCompanionComponent::UCompanionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCompanionComponent::BeginPlay()
{
	Super::BeginPlay();

	CompanionsSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UCompanionsSubsystem>();
	
	if(UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(FollowAction, ETriggerEvent::Started, this, &UCompanionComponent::OnFollow);
		Input->BindAction(UnFollowAction, ETriggerEvent::Started, this, &UCompanionComponent::OnUnFollow);
	}
	
	GetOwner()->GetWorldTimerManager().SetTimerForNextTick([this]
	{
		if (CompanionsSubsystem)
		{
			CompanionsSubsystem->TrySpawnCompanion(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UCompanionComponent::BeginPlay() Cant find Companion System"));
		}
	});
}

void UCompanionComponent::OnFollow(const FInputActionValue& InputActionValue)
{
	if (CompanionsSubsystem)
	{
		if (ACompanionAIController* AIController = Cast<ACompanionAIController>(CompanionsSubsystem->GetCompanion()->GetController()))
		{
			AIController->SetFollowing(true);
		}
	}
}

void UCompanionComponent::OnUnFollow(const FInputActionValue& InputActionValue)
{
	if (CompanionsSubsystem)
	{
		if (ACompanionAIController* AIController = Cast<ACompanionAIController>(CompanionsSubsystem->GetCompanion()->GetController()))
		{
			AIController->SetFollowing(false);
		}
	}
}

