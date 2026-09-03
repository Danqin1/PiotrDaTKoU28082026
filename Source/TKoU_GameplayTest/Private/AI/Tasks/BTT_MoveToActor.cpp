// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_MoveToActor.h"

#include "AIController.h"
#include "AI/GT_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/AIInterface.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveToActor::UBTT_MoveToActor()
{
	NodeName = "MoveToActor";
}

EBTNodeResult::Type UBTT_MoveToActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AGT_AIController* Controller = Cast<AGT_AIController>(OwnerComp.GetAIOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	if (UPathFollowingComponent* PathFollowingComponent = Controller->GetPathFollowingComponent())
	{
		const bool bIsUsingNavLink = PathFollowingComponent->IsFollowingNavLink()
			|| PathFollowingComponent->IsCurrentSegmentNavigationLink()
			|| PathFollowingComponent->HasStartedNavLinkMove();

		if (bIsUsingNavLink)
		{
			return EBTNodeResult::Succeeded;
		}
	}

	const UAISettings* Settings = IAIInterface::Execute_GetSettings(Controller);
	if (!Settings)
	{
		return EBTNodeResult::Failed;
	}

	Controller->MoveToActor(Actor, Settings->FollowPlayerRadius / FollowDeadZoneScale);
	return EBTNodeResult::Succeeded;
}
