// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_MoveToActor.h"

#include "AIController.h"
#include "AI/GT_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_MoveToActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
	{
		if (AGT_AIController* Controller = Cast<AGT_AIController>(OwnerComp.GetAIOwner()))
		{
			Controller->MoveToActor(Actor, Controller->GetSettings()->FollowPlayerRadius);
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return  EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
