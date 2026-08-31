#pragma once

UENUM(BlueprintType)
enum class EGameplayPlayerState : uint8
{
	Nothing,
	Cutscene,
	Dead,
	Interaction
};