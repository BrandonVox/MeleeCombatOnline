// Copyright Brandon Vox.


#include "Component/LocalCosmeticComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueManager.h"
#include "Data/DataAsset_Trace.h"
#include "GAS/MCOGameplayTag.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
ULocalCosmeticComponent::ULocalCosmeticComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULocalCosmeticComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// dedicated server does not render game 
	// skip cosmetic fx
	// net mode
	if (IsNetMode(NM_DedicatedServer))
	{
		return;
	}
	

	IAbilitySystemInterface* OwnerASI = GetOwner<IAbilitySystemInterface>();

	if (OwnerASI == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* OwnerASC = OwnerASI->GetAbilitySystemComponent();

	if (OwnerASC == nullptr)
	{
		return;
	}

	// work with exact gameplay tag
	OwnerASC->GenericGameplayEventCallbacks.FindOrAdd(MCOGameplayTag::Event_HitDetection_Begin)
	        .AddUObject(this, &ULocalCosmeticComponent::HandleHitDetection_Begin);

	OwnerASC->GenericGameplayEventCallbacks.FindOrAdd(MCOGameplayTag::Event_HitDetection_End)
	        .AddUObject(this, &ULocalCosmeticComponent::HandleHitDetection_End);

	OwnerASC->GenericGameplayEventCallbacks.FindOrAdd(MCOGameplayTag::Event_HitDetection_Tick)
	        .AddUObject(this, &ULocalCosmeticComponent::HandleHitDetection_Tick);
}

void ULocalCosmeticComponent::HandleHitDetection_Begin(const FGameplayEventData* EventData)
{
	// UE_LOG(LogTemp, Warning, TEXT("Local Cosmetic: Hit Detection _ Begin"));
	ActorsHitThisSwing.Empty();

	USkeletalMeshComponent* MeshComp = GetOwnerMeshComponent();
	if (MeshComp)
	{
		PrevTraceStart = MeshComp->GetSocketLocation(TraceData->SocketName_Start);
		PrevTraceEnd = MeshComp->GetSocketLocation(TraceData->SocketName_End);
	}
}

void ULocalCosmeticComponent::HandleHitDetection_End(const FGameplayEventData* EventData)
{
	// UE_LOG(LogTemp, Warning, TEXT("Local Cosmetic: Hit Detection _ End"));
	ActorsHitThisSwing.Empty();
}

void ULocalCosmeticComponent::HandleHitDetection_Tick(const FGameplayEventData* EventData)
{
	// UE_LOG(LogTemp, Warning, TEXT("Local Cosmetic: Hit Detection _ Tick"));
	USkeletalMeshComponent* MeshComp = GetOwnerMeshComponent();

	if (MeshComp == nullptr)
	{
		return;
	}

	FVector CurrentTraceStart = MeshComp->GetSocketLocation(TraceData->SocketName_Start);
	FVector CurrentTraceEnd = MeshComp->GetSocketLocation(TraceData->SocketName_End);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FLinearColor TraceColor_Local = TraceColor;

	// Perform trace for current frame
	PerformTraceAndProcessHitResults(CurrentTraceStart, CurrentTraceEnd, ActorsToIgnore, TraceColor_Local);
	FillTraceGap(CurrentTraceStart, CurrentTraceEnd, ActorsToIgnore);
	PrevTraceStart = CurrentTraceStart;
	PrevTraceEnd = CurrentTraceEnd;
}

void ULocalCosmeticComponent::ProcessHitResults(const TArray<FHitResult>& GivenHitResults)
{
	for (const FHitResult& HitResult : GivenHitResults)
	{
		AActor* VictimActor = HitResult.GetActor();
		if (VictimActor == nullptr)
		{
			continue;
		}

		if (ActorsHitThisSwing.Contains(VictimActor))
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Victim Actor Name: %s"), *VictimActor->GetName());
		// Play Local Gameplay Cue
		UGameplayCueManager* GCM = UAbilitySystemGlobals::Get().GetGameplayCueManager();
		FGameplayCueParameters GameplayCueParam_Impact;
		GameplayCueParam_Impact.Location = HitResult.ImpactPoint;
		if (GCM)
		{
			GCM->HandleGameplayCue
			(
				VictimActor,
				MCOGameplayTag::GameplayCue_BasicAttack_Hit_Impact_Aurora,
				EGameplayCueEvent::Executed,
				GameplayCueParam_Impact
			);
		}

		ActorsHitThisSwing.Add(VictimActor);
	}
}

void ULocalCosmeticComponent::PerformTraceAndProcessHitResults(FVector TraceStart, FVector TraceEnd,
                                                               const TArray<AActor*>& ActorsToIgnore,
                                                               FLinearColor GivenTraceColor)
{
	const EDrawDebugTrace::Type DrawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		this,
		TraceStart,
		TraceEnd,
		TraceData->TraceRadius,
		TraceData->TraceObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		HitResults,
		false,
		GivenTraceColor,
		TraceHitColor,
		DrawTime
	);

	ProcessHitResults(HitResults);
}

void ULocalCosmeticComponent::FillTraceGap(FVector CurrentTraceStart, FVector CurrentTraceEnd,
                                           const TArray<AActor*>& ActorsToIgnore)
{
	// Fill End
	float TwoEndDistance = FVector::Distance(PrevTraceEnd, CurrentTraceEnd);
	float FillStep_End = TraceData->TraceRadius * 1.3f;

	int32 FillCount = FMath::TruncToInt32(TwoEndDistance / FillStep_End);
	if (FillCount == 0)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("FillCount = %d"), FillCount);

	FVector FillDirection_End = (CurrentTraceEnd - PrevTraceEnd).GetSafeNormal();


	// Fill Start

	float TwoStartDistance = FVector::Distance(PrevTraceStart, CurrentTraceStart);
	float FillStep_Start = TwoStartDistance / (FillCount + 1);
	FVector FillDirection_Start = (CurrentTraceStart - PrevTraceStart).GetSafeNormal();


	float CorrectCapsuleLength = FVector::Distance(CurrentTraceStart, CurrentTraceEnd);

	// Loop
	FillCount = FMath::Clamp(FillCount, 0, TraceData->MaxFillCount);
	for (int32 i = 1; i <= FillCount; ++i)
	{
		FVector FillTraceEnd = PrevTraceEnd + (FillStep_End * FillDirection_End * i);
		FVector FillTraceStart = PrevTraceStart + (FillStep_Start * FillDirection_Start * i);

		FVector CapsuleDirection = (FillTraceEnd - FillTraceStart).GetSafeNormal();

		FillTraceEnd = FillTraceStart + (CapsuleDirection * CorrectCapsuleLength);
		PerformTraceAndProcessHitResults(FillTraceStart, FillTraceEnd, ActorsToIgnore, TraceColor_FillGap);
	}
}

USkeletalMeshComponent* ULocalCosmeticComponent::GetOwnerMeshComponent()
{
	USkeletalMeshComponent* OwnerMeshComponent_RawPtr = OwnerMeshComponent.Get();
	if (OwnerMeshComponent_RawPtr == nullptr)
	{
		if (AActor* OwnerActor = GetOwner())
		{
			OwnerMeshComponent_RawPtr = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();
			OwnerMeshComponent = OwnerMeshComponent_RawPtr;
		}
	}
	return OwnerMeshComponent_RawPtr;
}
