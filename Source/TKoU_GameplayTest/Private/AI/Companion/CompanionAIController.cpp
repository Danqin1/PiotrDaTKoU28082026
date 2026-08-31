#include "AI/Companion/CompanionAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ACompanionAIController::ACompanionAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACompanionAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACompanionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (DefaultBehaviorTree)
	{
		RunBehaviorTree(DefaultBehaviorTree);
	}
}

void ACompanionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACompanionAIController::SetFollowing(bool Following, AActor* toFollow)
{
	if (Following)
	{
		IAIInterface::Execute_SetState(this, EAIState::Follow);
	}
	else
	{
		IAIInterface::Execute_ResetState(this, EAIState::Follow);
	}
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsObject(TargetBlackboardKey, toFollow);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACompanionAIController::SetFollowing: Cant find blackboard"))
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Setting companion Following: %s"), *FString(Following ? TEXT("following") : TEXT("not following")));
}

void ACompanionAIController::StartNavLinkAction_Implementation(FVector start, FVector end)
{
	ACharacter* CompanionCharacter = Cast<ACharacter>(GetPawn());
	if (!CompanionCharacter)
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = CompanionCharacter->GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	const FVector JumpStart = CompanionCharacter->GetActorLocation();
	FVector DirectionToDestination = end - JumpStart;
	DirectionToDestination.Z = 0.0f;

	if (!DirectionToDestination.IsNearlyZero())
	{
		const FRotator CurrentRotation = CompanionCharacter->GetActorRotation();
		const float TargetYaw = DirectionToDestination.Rotation().Yaw;
		CompanionCharacter->SetActorRotation(FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll));
	}

	const float Gravity = FMath::Max(FMath::Abs(MovementComponent->GetGravityZ()), 1.0f);
	const float HeightDifference = end.Z - JumpStart.Z;
	const float ApexHeight = FMath::Max(120.0f, HeightDifference + 80.0f);
	const float VerticalSpeed = FMath::Sqrt(2.0f * Gravity * ApexHeight);
	const float Discriminant = FMath::Max(0.0f, FMath::Square(VerticalSpeed) - (2.0f * Gravity * HeightDifference));
	const float FlightTime = FMath::Max((VerticalSpeed + FMath::Sqrt(Discriminant)) / Gravity, 0.1f);

	const FVector HorizontalVelocity = DirectionToDestination / FlightTime;
	const FVector LaunchVelocity = HorizontalVelocity + FVector::UpVector * VerticalSpeed * 1.3f;

	MovementComponent->StopMovementImmediately();
	CompanionCharacter->LaunchCharacter(LaunchVelocity, true, true);
}
