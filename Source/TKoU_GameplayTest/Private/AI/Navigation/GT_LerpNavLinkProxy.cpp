#include "AI/Navigation/GT_LerpNavLinkProxy.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavLinkCustomComponent.h"
#include "Interfaces/NavInterface.h"

AGT_LerpNavLinkProxy::AGT_LerpNavLinkProxy()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AGT_LerpNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();

	if (UNavLinkCustomComponent* SmartLink = GetSmartLinkComp())
	{
		SmartLink->SetMoveReachedLink(this, &AGT_LerpNavLinkProxy::OnSmartLinkReached);
	}
}

void AGT_LerpNavLinkProxy::OnSmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& Destination)
{
	UPathFollowingComponent* PathFollowingComponent = Cast<UPathFollowingComponent>(PathingAgent);
	if (!PathFollowingComponent)
	{
		return;
	}

	AActor* Agent = PathFollowingComponent->GetOwner();
	if (AController* Controller = Cast<AController>(Agent))
	{
		Agent = Controller->GetPawn();
	}

	if (!Agent)
	{
		return;
	}

	const FVector Start = Agent->GetActorLocation();
	if (Destination.Z <= Start.Z + MinUpHeight)
	{
		ResumePathFollowing(Agent);
		return;
	}

	UObject* NavActionTarget = nullptr;
	if (AController* Controller = Cast<AController>(PathFollowingComponent->GetOwner()))
	{
		if (Controller->GetClass()->ImplementsInterface(UNavInterface::StaticClass()))
		{
			NavActionTarget = Controller;
		}
	}

	if (!NavActionTarget && Agent->GetClass()->ImplementsInterface(UNavInterface::StaticClass()))
	{
		NavActionTarget = Agent;
	}

	if (!NavActionTarget)
	{
		ResumePathFollowing(Agent);
		return;
	}

	INavInterface::Execute_StartNavLinkAction(NavActionTarget, Start, Destination);
}
