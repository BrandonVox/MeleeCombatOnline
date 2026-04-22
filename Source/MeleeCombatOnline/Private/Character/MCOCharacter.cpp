// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/WidgetComponent_Overhead.h"
#include "GAS/Attribute/AttributeSet_Base.h"
#include "GAS/Component/MCO_ASC.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetNetUpdateFrequency(64.f);

	// ASC
	MCO_ASC = CreateDefaultSubobject<UMCO_ASC>(TEXT("MCO_ASC"));
	MCO_ASC->SetIsReplicated(true);
	MCO_ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Attribute Set
	AttributeSet_Base = CreateDefaultSubobject<UAttributeSet_Base>(TEXT("AttributeSet_Base"));
	
	// Widget Component _ Overhead
	WidgetComponent_Overhead = CreateDefaultSubobject<UWidgetComponent_Overhead>(TEXT("WidgetComponent_Overhead"));
	WidgetComponent_Overhead->SetupAttachment(GetRootComponent());
	WidgetComponent_Overhead->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	WidgetComponent_Overhead->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent_Overhead->SetDrawAtDesiredSize(true);
	// user widget class -> update in blueprint
	
	// Mesh Component
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
		MCO_ASC->ApplyInitialEffects();
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
