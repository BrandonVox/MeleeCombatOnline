// Copyright Brandon Vox.


#include "GAS/Ability/GA_BasicAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/MCOGameplayTag.h"

UGA_BasicAttack::UGA_BasicAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateAbility C++"));

	// Play Attack Montage
	UAbilityTask_PlayMontageAndWait* Task_PlayMontage_Attack =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			AttackMontage,
			1.f,
			NAME_None,
			true
		);

	Task_PlayMontage_Attack->OnCompleted.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->OnBlendOut.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->OnInterrupted.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->OnCancelled.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
	Task_PlayMontage_Attack->ReadyForActivation();

	// Open Combo Window
	UAbilityTask_WaitGameplayEvent* Task_Event_OpenComboWindow = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent
	(
		this,
		MCOGameplayTag::Event_BasicAttack_Combo_Open,
		nullptr,
		false,
		false
	);
	Task_Event_OpenComboWindow->EventReceived.AddDynamic(this, &UGA_BasicAttack::ComboWindowOpened);
	Task_Event_OpenComboWindow->ReadyForActivation();

	// Close Combo Window
	UAbilityTask_WaitGameplayEvent* Task_Event_CloseComboWindow = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent
	(
		this,
		MCOGameplayTag::Event_BasicAttack_Combo_Close,
		nullptr,
		false,
		true
	);
	Task_Event_CloseComboWindow->EventReceived.AddDynamic(this, &UGA_BasicAttack::ComboWindowClosed);
	Task_Event_CloseComboWindow->ReadyForActivation();
}

// Not Function Callback
// Called Function
// Might be called multiple times
void UGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                 bool bWasCancelled)
{
	if (bIsActive)
	{
		// only run once
		UE_LOG(LogTemp, Warning, TEXT("EndAbility C++"));
		SectionName_Next = NAME_None;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_BasicAttack::ComboWindowOpened(FGameplayEventData EventData)
{
	SectionName_Next = EventData.EventTag.GetTagLeafName();

	UE_LOG(LogTemp, Warning, TEXT("ComboWindowOpened: %s"), *SectionName_Next.ToString());
}

void UGA_BasicAttack::ComboWindowClosed(FGameplayEventData EventData)
{
	SectionName_Next = MCOGameplayTag::Event_BasicAttack_Combo_Open_1.GetTag().GetTagLeafName();

	UE_LOG(LogTemp, Warning, TEXT("ComboWindowClosed"));
}
