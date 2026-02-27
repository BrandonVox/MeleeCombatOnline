// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MCOAnimNotify_Test.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UMCOAnimNotify_Test : public UAnimNotify
{
	GENERATED_BODY()

public:
	UMCOAnimNotify_Test();

	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
