// Fill out your copyright notice in the Description page of Project Settings.


#include "Companion/Companion.h"

ACompanion::ACompanion()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACompanion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACompanion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

