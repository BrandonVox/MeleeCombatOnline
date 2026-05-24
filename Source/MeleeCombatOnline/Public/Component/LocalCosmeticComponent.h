// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocalCosmeticComponent.generated.h"


class UDataAsset_Trace;
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
	
	void ProcessHitResults(const TArray<FHitResult>& GivenHitResults);

	void PerformTraceAndProcessHitResults
	(
		FVector TraceStart,
		FVector TraceEnd,
		const TArray<AActor*>& ActorsToIgnore,
		FLinearColor GivenTraceColor
	);

	void FillTraceGap
	(
		FVector CurrentTraceStart,
		FVector CurrentTraceEnd,
		const TArray<AActor*>& ActorsToIgnore
	);
	
	USkeletalMeshComponent* GetOwnerMeshComponent();
	
private: // Property
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ActorsHitThisSwing;
	
	UPROPERTY()
	TWeakObjectPtr<USkeletalMeshComponent> OwnerMeshComponent;
	
	FVector PrevTraceStart;
	FVector PrevTraceEnd;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	TObjectPtr<UDataAsset_Trace> TraceData;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	bool bDrawDebugTrace = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	float DrawTime = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceColor = FLinearColor::White;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceColor_FillGap = FLinearColor::Gray;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceHitColor = FLinearColor::Green;
	
};
