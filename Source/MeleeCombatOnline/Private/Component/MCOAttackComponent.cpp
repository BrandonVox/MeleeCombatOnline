// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMCOAttackComponent::UMCOAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UMCOAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UMCOAttackComponent, bIsAttacking);

	// replicate condition -> to all clients
	// onrep notify condition
	// trigger every times
	// even if the new value is the same on client
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttackComponent, bIsAttacking, COND_None, REPNOTIFY_Always);

	// bisattacking -> replicate
	// attack component -> replicate
	// MCO Character -> replicate
}

void UMCOAttackComponent::BeginPlay()
{
	Super::BeginPlay();
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
	// only run server
	TryAttack();
}

void UMCOAttackComponent::TryAttack()
{
	if (bIsAttacking)
	{
		return;
	}

	if (HasAuthority(GetOwner()))
	{
		bIsAttacking = true;
		OnSet_bIsAttacking();

		// server update bisattacking: true -> false || false -> true
		// server replicates -> clients
		// OnRep notify
		// Default: OnRep only call if new value != old value
		// new bisattacking = true
		// old bisattacking = true
		// on rep will not trigger
	}
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
	// true
	// true
	// repnotify -> is not trigger
	// false
	// true
	// true -> false
	// repnotify trigger
	if (HasAuthority(GetOwner()))
	{
		bIsAttacking = false;
		OnSet_bIsAttacking();
	}
}

bool UMCOAttackComponent::HasAuthority(const AActor* InActor)
{
	return InActor && InActor->HasAuthority();
}

void UMCOAttackComponent::OnRep_bIsAttacking(bool bOldValue)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_bIsAttacking"));
	// new value bisattacking
	// react to the change of bisattacking from client
	// new value == old value

	if (bIsAttacking != bOldValue)
	{
		OnSet_bIsAttacking();
	}
}
