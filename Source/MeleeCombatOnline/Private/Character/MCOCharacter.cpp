// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/MCOAttackComponent.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AttackComponent = CreateDefaultSubobject<UMCOAttackComponent>(TEXT("Attack Component"));
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

