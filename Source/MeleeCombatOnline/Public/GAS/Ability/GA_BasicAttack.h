// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BasicAttack.generated.h"

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
	void HandleReceiveEvent_EndAttack(FGameplayEventData EventData);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings")
	TObjectPtr<UAnimMontage> AttackMontage;
};
