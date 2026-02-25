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
	if (!CanAttack())
	{
		return;
	}

	if (HasAuthority(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bIsAttacking = true;
		CurrentState.bComboWindowOpened = false;
		++CurrentState.AttackCount;

		HandleCurrentStateChanged(OldState);
	}
}

bool UMCOAttackComponent::CanAttack() const
{
	if (!CurrentState.bIsAttacking)
	{
		return true;
	}

	if (CurrentState.bComboWindowOpened)
	{
		return true;
	}

	return false;
}

void UMCOAttackComponent::HandleCurrentStateChanged(const FAttackState& OldState)
{
	if (CurrentState.bIsAttacking && CurrentState.AttackCount > OldState.AttackCount)
	{
		GetOwner<ACharacter>()->PlayAnimMontage(GetAttackMontage(CurrentState.AttackCount, CurrentState.IndexOffset));
	}
}

UAnimMontage* UMCOAttackComponent::GetAttackMontage(const uint16 InAttackCount, const uint16 IndexOffset) const
{
	if (InAttackCount == 0)
	{
		return nullptr;
	}

	if (Montages_Attack.IsEmpty())
	{
		return nullptr;
	}

	const uint16 CalculatedIndex = (InAttackCount - IndexOffset) % Montages_Attack.Num();

	return Montages_Attack[CalculatedIndex];
}

void UMCOAttackComponent::OpenComboWindow()
{
	if (HasAuthority(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bComboWindowOpened = true;

		HandleCurrentStateChanged(OldState);
	}
}

void UMCOAttackComponent::EndAttack()
{
	if (HasAuthority(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bIsAttacking = false;
		CurrentState.bComboWindowOpened = false;
		// Reset Attack Index -> 0
		CurrentState.IndexOffset = CurrentState.AttackCount + 1;

		HandleCurrentStateChanged(OldState);
	}
}

bool UMCOAttackComponent::HasAuthority(const AActor* InActor)
{
	return InActor && InActor->HasAuthority();
}

void UMCOAttackComponent::OnRep_CurrentState(const FAttackState& OldState)
{
	HandleCurrentStateChanged(OldState);
}
