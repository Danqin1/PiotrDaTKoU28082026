#pragma once

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Follow,
	Search,
	Dead
};
