// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GT_GameInstance.h"
#include "Core/GT_GameInstanceSubsystem.h"

void UGT_GameInstance::Init()
{
	Super::Init();
	
	ForEachSubsystem<UGT_GameInstanceSubsystem>([](UGT_GameInstanceSubsystem* Subsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Subsystem: %s"), *GetNameSafe(Subsystem));
		Subsystem->PostInit();
	});
}
