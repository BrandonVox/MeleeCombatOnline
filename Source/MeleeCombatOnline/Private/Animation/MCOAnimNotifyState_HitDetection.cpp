// Copyright Brandon Vox.


#include "Animation/MCOAnimNotifyState_HitDetection.h"

#include "Component/MCOAttackComponent.h"

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
	
	if (!MeshComp)
	{
		return;
	}

	if (!MeshComp->GetOwner())
	{
		return;
	}

	UMCOAttackComponent* OwnerAttackComponent = MeshComp->GetOwner()->FindComponentByClass<UMCOAttackComponent>();
	if (OwnerAttackComponent)
	{
		OwnerAttackComponent->BeginHitDetection();
	}
}

void UMCOAnimNotifyState_HitDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
	{
		return;
	}

	if (!MeshComp->GetOwner())
	{
		return;
	}

	UMCOAttackComponent* OwnerAttackComponent = MeshComp->GetOwner()->FindComponentByClass<UMCOAttackComponent>();
	if (OwnerAttackComponent)
	{
		OwnerAttackComponent->EndHitDetection();
	}
}
