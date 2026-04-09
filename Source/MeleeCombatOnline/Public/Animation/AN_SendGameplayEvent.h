// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UAN_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
	
	
public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify
	(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

private:
	UPROPERTY(EditAnywhere, Category = "MCO Settings")
	FGameplayTag EventTag;
};
