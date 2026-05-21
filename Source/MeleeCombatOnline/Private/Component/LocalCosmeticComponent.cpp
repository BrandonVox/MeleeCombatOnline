// Copyright Brandon Vox.


#include "Component/LocalCosmeticComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/MCOGameplayTag.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Local Cosmetic: Hit Detection _ Begin"));
}

void ULocalCosmeticComponent::HandleHitDetection_End(const FGameplayEventData* EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Local Cosmetic: Hit Detection _ End"));
}

void ULocalCosmeticComponent::HandleHitDetection_Tick(const FGameplayEventData* EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Local Cosmetic: Hit Detection _ Tick"));
}
