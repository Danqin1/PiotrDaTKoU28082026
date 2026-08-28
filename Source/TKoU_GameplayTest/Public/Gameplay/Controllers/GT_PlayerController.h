
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GT_PlayerController.generated.h"


class AGT_MadelaineCharacter;

UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	TObjectPtr<AGT_MadelaineCharacter> CachedMadelaineCharacter;
};
