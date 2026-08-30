#pragma once

UENUM(BlueprintType)
enum EAIState : uint8
{
	Idle,
	Follow,
	Search,
	Dead
};
