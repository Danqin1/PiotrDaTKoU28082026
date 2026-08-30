#include "Save/SaveSubsystem.h"

#include "Core/GT_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Save/PlayerSave.h"

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadPlayerSave(DefaultPlayerSaveSlotName, DefaultUserIndex);
}

void USaveSubsystem::Deinitialize()
{
	CurrentPlayerSave = nullptr;

	Super::Deinitialize();
}

void USaveSubsystem::PostInit()
{
	Super::PostInit();
	GetGameInstance()->ForEachSubsystem<UGT_GameInstanceSubsystem>([this](UGT_GameInstanceSubsystem* Subsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Subsystem: %s"), *GetNameSafe(Subsystem));
		Subsystem->Restore(CurrentPlayerSave);
	});
}

UPlayerSave* USaveSubsystem::CreateNewPlayerSave(const FString& SlotName, int32 UserIndex)
{
	UPlayerSave* NewPlayerSave = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
	if (!NewPlayerSave)
	{
		return nullptr;
	}

	NewPlayerSave->SaveSlotName = SlotName;
	NewPlayerSave->UserIndex = UserIndex;
	CurrentPlayerSave = NewPlayerSave;

	return CurrentPlayerSave;
}

UPlayerSave* USaveSubsystem::LoadPlayerSave(const FString& SlotName, int32 UserIndex)
{
	if (USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex))
	{
		CurrentPlayerSave = Cast<UPlayerSave>(LoadedSave);
		if (CurrentPlayerSave)
		{
			CurrentPlayerSave->SaveSlotName = SlotName;
			CurrentPlayerSave->UserIndex = UserIndex;
			UE_LOG(LogTemp, Log, TEXT("UPlayerSave* USaveSubsystem::LoadPlayerSave Loaded Save from slot: %s"), *CurrentPlayerSave->SaveSlotName);
			return CurrentPlayerSave;
		}
	}

	return CreateNewPlayerSave(SlotName, UserIndex);
}

bool USaveSubsystem::SavePlayerSave()
{
	if (!CurrentPlayerSave)
	{
		CurrentPlayerSave = CreateNewPlayerSave(DefaultPlayerSaveSlotName, DefaultUserIndex);
	}

	return CurrentPlayerSave
		&& UGameplayStatics::SaveGameToSlot(
			CurrentPlayerSave,
			CurrentPlayerSave->SaveSlotName,
			CurrentPlayerSave->UserIndex);
}

bool USaveSubsystem::DeletePlayerSave(const FString& SlotName, int32 UserIndex)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		return true;
	}

	const bool bDeleted = UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
	if (bDeleted && CurrentPlayerSave
		&& CurrentPlayerSave->SaveSlotName == SlotName
		&& CurrentPlayerSave->UserIndex == UserIndex)
	{
		CurrentPlayerSave = nullptr;
	}

	return bDeleted;
}

bool USaveSubsystem::DoesPlayerSaveExist(const FString& SlotName, int32 UserIndex) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}