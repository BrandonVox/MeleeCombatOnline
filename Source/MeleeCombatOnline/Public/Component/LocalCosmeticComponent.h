// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocalCosmeticComponent.generated.h"


struct FGameplayEventData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATONLINE_API ULocalCosmeticComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULocalCosmeticComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private: // Function
	
	void HandleHitDetection_Begin(const FGameplayEventData* EventData);
	void HandleHitDetection_End(const FGameplayEventData* EventData);
	void HandleHitDetection_Tick(const FGameplayEventData* EventData);
	
};
