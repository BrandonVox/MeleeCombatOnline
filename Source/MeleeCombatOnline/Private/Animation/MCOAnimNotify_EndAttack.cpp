// Copyright Brandon Vox.


#include "Animation/MCOAnimNotify_EndAttack.h"

#include "Component/MCOAttackComponent.h"

UMCOAnimNotify_EndAttack::UMCOAnimNotify_EndAttack()
{
	// bIsNativeBranchingPoint = true;
}

void UMCOAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
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
		// UE_LOG(LogTemp, Warning, TEXT("Notify End Attack"));
		OwnerAttackComponent->EndAttack();
	}
}
