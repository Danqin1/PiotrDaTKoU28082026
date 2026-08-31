// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_IsTooFarFromTarget.h"

#include "AIController.h"
#include "AI/GT_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/AIInterface.h"
#include "Settings/AISettings.h"

UBTT_IsTooFarFromTarget::UBTT_IsTooFarFromTarget()
{
	NodeName = "IsTooFarFromTarget";
}

EBTNodeResult::Type UBTT_IsTooFarFromTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Actor = Cast<AActor>(BlackboardComponent->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!Actor)
	{
		return EBTNodeResult::Failed;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller || !Controller->GetClass()->ImplementsInterface(UAIInterface::StaticClass()))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	const UAISettings* Settings = IAIInterface::Execute_GetSettings(Controller);
	if (!Settings)
	{
		return EBTNodeResult::Failed;
	}

	return FVector::Dist(Pawn->GetActorLocation(), Actor->GetActorLocation()) > Settings->FollowPlayerRadius
		? EBTNodeResult::Succeeded
		: EBTNodeResult::Failed;
}
