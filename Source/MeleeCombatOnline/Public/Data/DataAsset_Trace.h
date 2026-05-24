// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_Trace.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UDataAsset_Trace : public UDataAsset
{
	GENERATED_BODY()
	
public: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	FName SocketName_Start = FName(TEXT("Sword_Base"));

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	FName SocketName_End = FName(TEXT("Sword_Tip"));

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	float TraceRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	int32 MaxFillCount = 25;
};
