// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PlayerHUDWidget.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ClimbLabelBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerHUDWidget::ShowClimbAvailable(bool available)
{
	ClimbLabelBox->SetVisibility(available ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
