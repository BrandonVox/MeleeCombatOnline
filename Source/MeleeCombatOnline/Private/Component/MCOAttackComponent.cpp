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
	if (bIsAttacking)
	{
		return;
	}
	
	bIsAttacking = true;
	OnSet_bIsAttacking();
}

void UMCOAttackComponent::OnSet_bIsAttacking()
{
	if (bIsAttacking)
	{
		GetOwner<ACharacter>()->PlayAnimMontage(Montages_Attack[AttackIndex]);
		IncreaseAttackIndex();
	}
}

void UMCOAttackComponent::IncreaseAttackIndex()
{
	++AttackIndex;
	
	if (AttackIndex >= Montages_Attack.Num())
	{
		AttackIndex = 0;
	}
}

void UMCOAttackComponent::EndAttack()
{
	bIsAttacking = false;
}

