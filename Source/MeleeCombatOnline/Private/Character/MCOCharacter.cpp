// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/WidgetComponent_Overhead.h"
#include "GAS/Component/MCO_ASC.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetNetUpdateFrequency(64.f);

	MCO_ASC = CreateDefaultSubobject<UMCO_ASC>(TEXT("MCO_ASC"));
	MCO_ASC->SetIsReplicated(true);
	MCO_ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Widget Component _ Overhead
	WidgetComponent_Overhead = CreateDefaultSubobject<UWidgetComponent_Overhead>(TEXT("WidgetComponent_Overhead"));
	WidgetComponent_Overhead->SetupAttachment(GetRootComponent());
	WidgetComponent_Overhead->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	WidgetComponent_Overhead->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent_Overhead->SetDrawAtDesiredSize(true);
	// user widget class -> update in blueprint
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
		MCO_ASC->GiveDefaultAbilities();
	}
}

// Clients
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
