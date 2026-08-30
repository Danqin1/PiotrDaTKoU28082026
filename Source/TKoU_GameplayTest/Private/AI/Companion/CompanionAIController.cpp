// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Companion/CompanionAIController.h"


// Sets default values
ACompanionAIController::ACompanionAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACompanionAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACompanionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACompanionAIController::SetFollowing(bool Following)
{
}

