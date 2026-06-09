// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BasicAttack.generated.h"

class UDataAsset_Trace;
/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UGA_BasicAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BasicAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

private: // Function
	UFUNCTION()
	void ComboWindowOpened(FGameplayEventData EventData);

	UFUNCTION()
	void ComboWindowClosed(FGameplayEventData EventData);

	UFUNCTION()
	void HitDetectionRequested(FGameplayEventData EventData);

	UFUNCTION()
	void HitDetectionRequested_Begin(FGameplayEventData EventData);

	UFUNCTION()
	void HitDetectionRequested_End(FGameplayEventData EventData);

	UFUNCTION()
	void HitDetectionRequested_Tick(FGameplayEventData EventData);

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

	UFUNCTION()
	void HandleInput_Press(float TimeWaited);
	
	UFUNCTION()
	void HandleInput_Release(float TimeHeld);
	
	void JumpToNextSectionImmediately();
	
	void SetupWaitInput_Press(bool bAlreadyPressed);
	void SetupWaitInput_Release();
	
	void SetNextSection();
	
	void BindCallbackToSectionChangedDelegate();
	
	UFUNCTION()
	void HandleMontageSectionChanged(UAnimMontage* Montage, FName SectionName, bool bLooped);
	
	UFUNCTION()
	void HandleMontageBlendOut_Attack();

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings")
	TObjectPtr<UAnimMontage> Montage_Attack;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings")
	TObjectPtr<UAnimMontage> Montage_Recovery;
	
	bool bIsPressing = false;
	bool bConfirmPressInComboWindow = false;

	FName Name_Section_Next = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	TObjectPtr<UDataAsset_Trace> TraceData;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	bool bDrawDebugTrace = true;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	float DrawTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceColor_Server = FLinearColor::Blue;
	
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceColor_FillGap = FLinearColor::Yellow;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	FLinearColor TraceHitColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	TSubclassOf<UGameplayEffect> Class_GE_Damage;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ActorsHitThisSwing;

	FVector PrevTraceStart;
	FVector PrevTraceEnd;
};
