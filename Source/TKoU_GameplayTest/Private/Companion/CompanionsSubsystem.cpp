// Fill out your copyright notice in the Description page of Project Settings.


#include "Companion/CompanionsSubsystem.h"

#include "Companion/Companion.h"
#include "Companion/CompanionData.h"
#include "Companion/CompanionDatabase.h"
#include "Save/PlayerSave.h"

void UCompanionsSubsystem::PostInit()
{
	UE_LOG(LogTemp, Log, TEXT("GT Subsystem Initialized: %s"), *GetNameSafe(this));
	
	if (UCompanionDatabase* DB = LoadObject<UCompanionDatabase>(nullptr, TEXT("/Game/Data/AI/Companion/DA_Companions.DA_Companions")))
	{
		CompanionsDatabase = DB;
	}
	if (!CompanionsDatabase)
	{
		UE_LOG(LogTemp, Error, TEXT("UCompanionsSubsystem::PostInit() CompanionsDatabase not found"));
	}
}

void UCompanionsSubsystem::Restore(UPlayerSave* Save)
{
	PlayerSave = Save;
}

bool UCompanionsSubsystem::TrySpawnCompanion(AActor* Player)
{
	if (!PlayerSave)
	{
		UE_LOG(LogTemp, Error, TEXT(" UCompanionsSubsystem::TrySpawnCompanion(AActor* Player) Cant spawn companion - no player save"));
		return false;
	}
	if (PlayerSave->CompanionSaveData.ChosenCompanion.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT(" UCompanionsSubsystem::TrySpawnCompanion(AActor* Player) Cant spawn companion"));
		return false;
	}
	
	if (!CompanionsDatabase)
	{
		UE_LOG(LogTemp, Error, TEXT("UCompanionsSubsystem::TrySpawnCompanion(AActor* Player) CompanionsDatabase not found"));
		return false;
	}
	
	UCompanionData* data = CompanionsDatabase->GetCompanionData(PlayerSave->CompanionSaveData.ChosenCompanion);
	if (!data || ActiveCompanion)
	{
		UE_LOG(LogTemp, Error, TEXT(" UCompanionsSubsystem::TrySpawnCompanion(AActor* Player) Cant spawn companion"));
		return false;
	}
	
	FActorSpawnParameters spawnParams;
	ActiveCompanion = GetWorld()->SpawnActor<ACompanion>(data->Blueprint ,
		Player->GetActorLocation() + FVector::RightVector, FRotator::ZeroRotator, spawnParams);
	
	if (!ActiveCompanion)
	{
		UE_LOG(LogTemp, Error, TEXT(" UCompanionsSubsystem::TrySpawnCompanion(AActor* Player) Cant spawn companion"));
		return false;
	}
	return true;
}

ACompanion* UCompanionsSubsystem::GetCompanion()
{
	return ActiveCompanion;
}
