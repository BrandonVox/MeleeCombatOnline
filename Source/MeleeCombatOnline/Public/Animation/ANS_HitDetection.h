// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_HitDetection.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UANS_HitDetection : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_HitDetection();
	
	virtual FString GetNotifyName_Implementation() const override;
	
};
