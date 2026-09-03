#include "Player/Animation/NotifyPlayFootsteps.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"

void UNotifyPlayFootsteps::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Start = MeshComp->GetComponentLocation() + FVector::UpVector * TraceStartOffset;
	const FVector End = Start - FVector::UpVector * TraceDistance;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(NotifyPlayFootstepsTrace), false, MeshComp->GetOwner());
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredComponent(MeshComp);

	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);

	const UPhysicalMaterial* PhysicalMaterial = bHit ? Hit.PhysMaterial.Get() : nullptr;
	USoundBase* FootstepSound = DefaultSound;

	if (DirtMat && PhysicalMaterial == DirtMat)
	{
		FootstepSound = DirtSound;
	}
	else if (FloorMat && PhysicalMaterial == FloorMat)
	{
		FootstepSound = FloorSound;
	}

	if (FootstepSound)
	{
		const FVector SoundLocation = bHit ? Hit.ImpactPoint : MeshComp->GetComponentLocation();
		UGameplayStatics::PlaySoundAtLocation(World, FootstepSound, SoundLocation);
	}
}
