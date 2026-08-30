// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Companion/CompanionsSubsystem.h"
#include "Components/ActorComponent.h"
#include "CompanionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TKOU_GAMEPLAYTEST_API UCompanionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCompanionComponent();

protected:
	
	UPROPERTY(Transient)
	UCompanionsSubsystem* CompanionsSubsystem;
	
	UPROPERTY(EditAnywhere)
	UInputAction* FollowAction;

	UPROPERTY(EditAnywhere)
	UInputAction* UnFollowAction;
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnFollow(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnUnFollow(const FInputActionValue& InputActionValue);
};
