// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Settings/AISettings.h"
#include "GT_AIController.generated.h"

UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_AIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGT_AIController();

protected:
	UPROPERTY(EditAnywhere)
	UAISettings* Settings;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	UAISettings* GetSettings();
};
