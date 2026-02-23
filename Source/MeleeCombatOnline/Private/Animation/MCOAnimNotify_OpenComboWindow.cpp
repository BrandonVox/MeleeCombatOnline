// Copyright Brandon Vox.


#include "Animation/MCOAnimNotify_OpenComboWindow.h"

#include "Component/MCOAttackComponent.h"

UMCOAnimNotify_OpenComboWindow::UMCOAnimNotify_OpenComboWindow()
{
#if WITH_EDITOR
	NotifyColor = FColor::Green;
#endif
}

FString UMCOAnimNotify_OpenComboWindow::GetNotifyName_Implementation() const
{
	return FString(TEXT("Combo"));
}

void UMCOAnimNotify_OpenComboWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
		OwnerAttackComponent->OpenComboWindow();
	}
}
