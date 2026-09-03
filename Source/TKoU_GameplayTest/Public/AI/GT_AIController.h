// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIState.h"
#include "Interfaces/AIInterface.h"
#include "Settings/AISettings.h"
#include "GT_AIController.generated.h"

UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_AIController : public AAIController, public IAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGT_AIController();

protected:
	UPROPERTY(EditAnywhere)
	UAISettings* Settings;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AI")
	EAIState State = EAIState::Idle;
	
	FName StateBlackboardKey = "State";
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	virtual UAISettings* GetSettings_Implementation() const override;
	
	virtual void SetState_Implementation(EAIState NewState) override;
	
	virtual EAIState GetState_Implementation() const override;
	
	virtual void ResetState_Implementation(EAIState state) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
