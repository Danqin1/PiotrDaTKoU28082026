#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/NavLinkProxy.h"
#include "GT_LerpNavLinkProxy.generated.h"

class UNavLinkCustomComponent;

UCLASS(Blueprintable)
class TKOU_GAMEPLAYTEST_API AGT_LerpNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()
    	
    void OnSmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& Destination);
protected:
	UPROPERTY(EditAnywhere, Category = "Smart Link")
	float MinUpHeight = 20.0f;
	
	virtual void BeginPlay() override;
	
public:
	AGT_LerpNavLinkProxy();
};
