// Copyright Brandon Vox.


#include "GAS/Ability/GA_BasicAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MeleeCombatOnline/MeleeCombatOnline.h"

UGA_BasicAttack::UGA_BasicAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

void UGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	// UE_LOG(LogTemp, Warning, TEXT("ActivateAbility"));

// #if ENABLE_VISUAL_LOG
// 	UE_VLOG(
// 		GetOwner(),
// 		LogOnRepFalse,
// 		Error,
// 		TEXT("OnRep| %s -> %s | %d -> %d"),
// 		*OldAttacking,
// 		*CurrentAttacking,
// 		OldState.AttackCount,
// 		AttackState.AttackCount
// 	);
// #endif
	
	#if ENABLE_VISUAL_LOG
		UE_VLOG(
			GetAvatarActorFromActorInfo(),
			LogActivateGA,
			Warning,
			TEXT("ActivateGA")
		);
	#endif

	UAbilityTask_PlayMontageAndWait* Task_PlayMontage_Attack =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			BasicAttackMontage,
			1.f,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);

	Task_PlayMontage_Attack->OnCompleted.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->OnBlendOut.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->OnInterrupted.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->OnCancelled.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);

	Task_PlayMontage_Attack->ReadyForActivation();
}

void UGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                 bool bWasCancelled)
{
	// UE_LOG(LogTemp, Warning, TEXT("EndAbility"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
