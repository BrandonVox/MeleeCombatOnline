// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"

// Sets default values for this component's properties
UMCOAttackComponent::UMCOAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMCOAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMCOAttackComponent::LocalInputPressed()
{
}
