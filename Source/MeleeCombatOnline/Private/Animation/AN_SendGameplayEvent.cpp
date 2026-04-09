// Copyright Brandon Vox.


#include "Animation/AN_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return EventTag.GetTagLeafName().ToString();
}

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp == nullptr)
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (OwnerASC == nullptr)
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
	(
		OwnerActor,
		EventTag,
		FGameplayEventData()
	);
}
