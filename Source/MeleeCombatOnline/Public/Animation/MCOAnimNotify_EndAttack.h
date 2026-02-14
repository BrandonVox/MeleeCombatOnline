// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MCOAnimNotify_EndAttack.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UMCOAnimNotify_EndAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify
	(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};
