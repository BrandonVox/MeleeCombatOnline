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

UAbilitySystemComponent* AMCOCharacter::GetAbilitySystemComponent() const
{
	return MCO_ASC;
}

