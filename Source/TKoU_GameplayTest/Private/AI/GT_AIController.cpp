#include "AI/GT_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Debug/DevDebugSubsystem.h"


AGT_AIController::AGT_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGT_AIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGT_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAISettings* AGT_AIController::GetSettings_Implementation() const
{
	return Settings;
}

void AGT_AIController::SetState_Implementation(EAIState NewState)
{
	// this should check in AIKnowledge first if we can change state to new one,
	// simplified for the test
	
	State = NewState;
	UE_LOG(LogTemp, Warning, TEXT("Companion changed state to: %s"), *UEnum::GetValueAsString(State));
	
	if (auto BB = GetBlackboardComponent())
	{
		BB->SetValueAsEnum(StateBlackboardKey, static_cast<uint8>(State));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AGT_AIController::SetState_Implementation: Cant find blackboard"))
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UDevDebugSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UDevDebugSubsystem>())
		{
			DebugSubsystem->CompanionState = State;
		}
	}
}

EAIState AGT_AIController::GetState_Implementation() const
{
	return State;
}

void AGT_AIController::ResetState_Implementation(EAIState state)
{
	if (State == state)
	{
		IAIInterface::Execute_SetState(this, EAIState::Idle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant reset %s State of EAIState current %s"), *UEnum::GetValueAsString(state), *UEnum::GetValueAsString(State));
	}
}

