// Copyright Brandon Vox.


#include "GAS/Ability/GA_BasicAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GAS/MCOGameplayTag.h"
#include "Kismet/KismetSystemLibrary.h"

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


	// Hit Detection
	UAbilityTask_WaitGameplayEvent* Task_Event_HitDetection = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent
	(
		this,
		MCOGameplayTag::Event_HitDetection,
		nullptr,
		false,
		false
	);
	Task_Event_HitDetection->EventReceived.AddDynamic(this, &UGA_BasicAttack::HitDetectionRequested);
	Task_Event_HitDetection->ReadyForActivation();
}

void UGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                 bool bWasCancelled)
{
	if (bIsActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndAbility C++"));
		SectionName_Next = NAME_None;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_BasicAttack::ComboWindowOpened(FGameplayEventData EventData)
{
	SectionName_Next = EventData.EventTag.GetTagLeafName();

	UE_LOG(LogTemp, Warning, TEXT("ComboWindowOpened: %s"), *SectionName_Next.ToString());

	// Input Press
	UAbilityTask_WaitInputPress* Task_InputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	Task_InputPress->OnPress.AddDynamic(this, &UGA_BasicAttack::ComboInputPressed);
	Task_InputPress->ReadyForActivation();
}

void UGA_BasicAttack::ComboWindowClosed(FGameplayEventData EventData)
{
	SectionName_Next = MCOGameplayTag::Event_BasicAttack_Combo_Open_1.GetTag().GetTagLeafName();

	UE_LOG(LogTemp, Warning, TEXT("ComboWindowClosed"));
}

void UGA_BasicAttack::HitDetectionRequested(FGameplayEventData EventData)
{
	FGameplayTag GivenTag = EventData.EventTag;

	if (GivenTag.MatchesTagExact(MCOGameplayTag::Event_HitDetection_Begin))
	{
		HitDetectionRequested_Begin(EventData);
	}
	else if (GivenTag.MatchesTagExact(MCOGameplayTag::Event_HitDetection_End))
	{
		HitDetectionRequested_End(EventData);
	}
	else if (GivenTag.MatchesTagExact(MCOGameplayTag::Event_HitDetection_Tick))
	{
		HitDetectionRequested_Tick(EventData);
	}
}

void UGA_BasicAttack::HitDetectionRequested_Begin(FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("HitDetectionRequested_Begin"));
}

void UGA_BasicAttack::HitDetectionRequested_End(FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("HitDetectionRequested_End"));
}

void UGA_BasicAttack::HitDetectionRequested_Tick(FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("HitDetectionRequested_Tick"));


	// Socket Location
	// Mesh Comp
	USkeletalMeshComponent* MeshComp = GetOwningComponentFromActorInfo();
	
	if (MeshComp == nullptr)
	{
		return;
	}
	
	FVector TraceStart = MeshComp->GetSocketLocation(SocketName_Start);
	FVector TraceEnd = MeshComp->GetSocketLocation(SocketName_End);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	const EDrawDebugTrace::Type DrawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	FLinearColor TraceColor_Local = K2_HasAuthority() ? TraceColor_Server : TraceColor;
	
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		this,
		TraceStart,
		TraceEnd,
		TraceRadius,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		false,
		TraceColor_Local,
		TraceHitColor,
		DrawTime
	);
}

void UGA_BasicAttack::ComboInputPressed(float TimeWaited)
{
	UE_LOG(LogTemp, Warning, TEXT("InputPressed"));

	// Jump to Section
	if (SectionName_Next == NAME_None)
	{
		return;
	}
	USkeletalMeshComponent* MeshComp = GetOwningComponentFromActorInfo();
	if (MeshComp == nullptr)
	{
		return;
	}

	UAnimInstance* MyAnimInstance = MeshComp->GetAnimInstance();
	if (MyAnimInstance == nullptr)
	{
		return;
	}

	MyAnimInstance->Montage_JumpToSection(SectionName_Next, AttackMontage);
	SectionName_Next = NAME_None;
}
