// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/MCOAttackComponent.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	// replication
	// character on server send update to clients
	// how many times in 1 second
	SetMinNetUpdateFrequency(30.f);
	SetNetUpdateFrequency(100.f);
	// 30 -> 100 updates per second
	
	AttackComponent = CreateDefaultSubobject<UMCOAttackComponent>(TEXT("Attack Component"));
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

