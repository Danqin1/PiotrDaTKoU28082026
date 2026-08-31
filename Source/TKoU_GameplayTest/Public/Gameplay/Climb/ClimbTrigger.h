// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ClimbableInterface.h"
#include "ClimbTrigger.generated.h"

UCLASS()
class TKOU_GAMEPLAYTEST_API AClimbTrigger : public AActor, public IClimbableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClimbTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climb")
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb")
	UAnimMontage* AnimToPlay;

	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:
	virtual bool CanClimb_Implementation() override;
	virtual void Climb_Implementation(ACharacter* character) override;
	virtual float GetDuration_Implementation() override;
};
