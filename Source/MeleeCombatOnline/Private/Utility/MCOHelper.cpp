// Copyright Brandon Vox.


#include "Utility/MCOHelper.h"

#include "AbilitySystemBlueprintLibrary.h"

bool UMCOHelper::HasAuthority(const AActor* InActor)
{
	if (InActor == nullptr)
	{
		return false;
	}
	
	return InActor->HasAuthority();
}

void UMCOHelper::SendGameplayEvent(USkeletalMeshComponent* MeshComp, FGameplayTag EventTag)
{
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
