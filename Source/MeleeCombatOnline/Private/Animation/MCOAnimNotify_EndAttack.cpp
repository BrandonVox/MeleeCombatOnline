// Copyright Brandon Vox.


#include "Animation/MCOAnimNotify_EndAttack.h"

#include "Component/MCOAttackComponent.h"

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
		OwnerAttackComponent->EndAttack();
	}
}
