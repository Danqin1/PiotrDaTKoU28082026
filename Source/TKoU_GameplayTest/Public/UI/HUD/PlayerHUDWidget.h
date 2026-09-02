// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TKOU_GAMEPLAYTEST_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UHorizontalBox* ClimbLabelBox;
	
	virtual void NativeConstruct() override;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void ShowClimbAvailable(bool available);
};
