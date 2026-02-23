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
	
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttackComponent, bIsAttacking, COND_None, REPNOTIFY_Always);
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

	if (bIsAttacking != bOldValue)
	{
		OnSet_bIsAttacking();
	}
}
