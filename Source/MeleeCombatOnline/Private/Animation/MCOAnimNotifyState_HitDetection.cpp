// Copyright Brandon Vox.


#include "Animation/MCOAnimNotifyState_HitDetection.h"

#include "Kismet/KismetSystemLibrary.h"

UMCOAnimNotifyState_HitDetection::UMCOAnimNotifyState_HitDetection()
{
#if WITH_EDITOR
	NotifyColor = FColor::Red;
#endif
}

FString UMCOAnimNotifyState_HitDetection::GetNotifyName_Implementation() const
{
	return FString(TEXT("HitDetection"));
}

void UMCOAnimNotifyState_HitDetection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UMCOAnimNotifyState_HitDetection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// FVector TraceStart;
	// FVector TraceEnd;
	// float TraceRadius;
	// TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	// TArray<AActor*> ActorsToIgnore;
	// EDrawDebugTrace::Type TraceDebugType;
	// TArray<FHitResult> HitResults;
	// UKismetSystemLibrary::SphereTraceMultiForObjects
	// (
	// 	MeshComp->GetOwner(),
	// 	TraceStart,
	// 	TraceEnd,
	// 	TraceRadius,
	// 	TraceObjectTypes,
	// 	false,
	// 	ActorsToIgnore,
	// 	TraceDebugType,
	// 	HitResults,
	// 	true
	// );
}

void UMCOAnimNotifyState_HitDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
