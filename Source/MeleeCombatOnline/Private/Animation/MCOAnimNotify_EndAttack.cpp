// Copyright Brandon Vox.


#include "Animation/MCOAnimNotify_EndAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/MCOGameplayTag.h"

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
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (!OwnerASC)
	{
		return;
	}

	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
	// (
	// 	MeshComp->GetOwner(),
	// 	MCOGameplayTag::Event_EndAttack,
	// 	FGameplayEventData()
	// );
}
