// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCOHelper.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UMCOHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool HasAuthority(const AActor* InActor);
	
	static void SendGameplayEvent(USkeletalMeshComponent* MeshComp, FGameplayTag EventTag);
	
};
