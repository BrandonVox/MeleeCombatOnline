// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MCOAnimNotify_OpenComboWindow.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UMCOAnimNotify_OpenComboWindow : public UAnimNotify
{
	GENERATED_BODY()

public:
	UMCOAnimNotify_OpenComboWindow();
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify
	(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};
