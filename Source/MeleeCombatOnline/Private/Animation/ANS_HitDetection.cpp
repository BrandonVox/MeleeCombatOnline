// Copyright Brandon Vox.


#include "Animation/ANS_HitDetection.h"

#include "GAS/MCOGameplayTag.h"
#include "Utility/MCOHelper.h"

UANS_HitDetection::UANS_HitDetection()
{
#if WITH_EDITOR
	NotifyColor = FColor::Red;
#endif 
}

FString UANS_HitDetection::GetNotifyName_Implementation() const
{
	return FString(TEXT("HitDetection"));
}

void UANS_HitDetection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	UMCOHelper::SendGameplayEvent(MeshComp, MCOGameplayTag::Event_HitDetection_Begin);
	
}

void UANS_HitDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	UMCOHelper::SendGameplayEvent(MeshComp, MCOGameplayTag::Event_HitDetection_End);
}

void UANS_HitDetection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	UMCOHelper::SendGameplayEvent(MeshComp, MCOGameplayTag::Event_HitDetection_Tick);
}
