// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "GAS/Component/MCO_ASC.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetNetUpdateFrequency(64.f);

	MCO_ASC = CreateDefaultSubobject<UMCO_ASC>(TEXT("MCO_ASC"));
	MCO_ASC->SetIsReplicated(true);
	MCO_ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Server
void AMCOCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (MCO_ASC)
	{
		MCO_ASC->InitAbilityActorInfo(this, this);
	}
}

void AMCOCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (MCO_ASC)
	{
		MCO_ASC->InitAbilityActorInfo(this, this);
	}
}

UAbilitySystemComponent* AMCOCharacter::GetAbilitySystemComponent() const
{
	return MCO_ASC;
}
