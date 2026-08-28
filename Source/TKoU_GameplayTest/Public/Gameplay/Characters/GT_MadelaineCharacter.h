// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GT_MadelaineCharacter.generated.h"


UCLASS()
class TKOU_GAMEPLAYTEST_API AGT_MadelaineCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AGT_MadelaineCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AddForwardMovementInput(float Value);
};
