// Copyright Brandon Vox.


#include "GAS/Ability/GA_BasicAttack.h"

void UGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateAbility"));
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}