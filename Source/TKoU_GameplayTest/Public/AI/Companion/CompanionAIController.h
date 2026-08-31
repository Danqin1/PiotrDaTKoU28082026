// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GT_AIController.h"
#include "Interfaces/NavInterface.h"
#include "CompanionAIController.generated.h"

class UBehaviorTree;

UCLASS()
class TKOU_GAMEPLAYTEST_API ACompanionAIController : public AGT_AIController, public INavInterface
{
	GENERATED_BODY()

public:
	
	ACompanionAIController();

protected:
	
	UPROPERTY(EditAnywhere)
	UBehaviorTree* DefaultBehaviorTree;
	
	FName TargetBlackboardKey = "PlayerTarget";

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void StartNavLinkAction_Implementation(FVector start, FVector end) override;
	
public:
	
	virtual void Tick(float DeltaTime) override;
	void SetFollowing(bool Following, AActor* toFollow);
};
