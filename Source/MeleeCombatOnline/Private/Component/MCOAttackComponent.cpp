// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UMCOAttackComponent::UMCOAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UMCOAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttackComponent, CurrentState, COND_None, REPNOTIFY_OnChanged);
}

void UMCOAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority(GetOwner()))
	{
		IncreaseAttackIndex();
	}
	
}

void UMCOAttackComponent::LocalInputPressed()
{
	// Listen Server
	if (HasAuthority(GetOwner()))
	{
		TryAttack();
		return;
	}

	// Client
	Server_TryAttack();
}

void UMCOAttackComponent::Server_TryAttack_Implementation()
{
	TryAttack();
}

void UMCOAttackComponent::TryAttack()
{
	if (CurrentState.bIsAttacking)
	{
		return;
	}

	if (HasAuthority(GetOwner()))
	{
		CurrentState.bIsAttacking = true;
		HandleCurrentStateChanged();
		// is attacking = true
		// index = 1
		// attack count!!
	}
}

void UMCOAttackComponent::HandleCurrentStateChanged()
{
	if (CurrentState.bIsAttacking)
	{
		GetOwner<ACharacter>()->PlayAnimMontage(Montages_Attack[CurrentState.AttackIndex]);
		IncreaseAttackIndex();
	}
}

void UMCOAttackComponent::IncreaseAttackIndex()
{
	++CurrentState.AttackIndex;

	if (CurrentState.AttackIndex >= Montages_Attack.Num())
	{
		CurrentState.AttackIndex = 0;
	}
}

void UMCOAttackComponent::EndAttack()
{
	if (HasAuthority(GetOwner()))
	{
		CurrentState.bIsAttacking = false;
		HandleCurrentStateChanged();
	}
}

bool UMCOAttackComponent::HasAuthority(const AActor* InActor)
{
	return InActor && InActor->HasAuthority();
}

void UMCOAttackComponent::OnRep_CurrentState(const FAttackState& OldState)
{
	HandleCurrentStateChanged();
}
