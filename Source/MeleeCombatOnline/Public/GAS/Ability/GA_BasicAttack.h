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

protected: // Function
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
