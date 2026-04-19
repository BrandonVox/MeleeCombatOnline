// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCOHelper.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UMCOHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool HasAuthority(const AActor* InActor);
	
};
