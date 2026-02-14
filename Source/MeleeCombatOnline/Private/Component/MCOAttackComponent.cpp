// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"

#include "GameFramework/Character.h"

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
	TryAttack();
}

void UMCOAttackComponent::TryAttack()
{
	// Play Montage
	GetOwner<ACharacter>()->PlayAnimMontage(Montages_Attack[0]);
}
