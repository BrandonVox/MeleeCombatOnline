// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/MCOAttackComponent.h"
#include "Components/CapsuleComponent.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetMinNetUpdateFrequency(30.f);
	SetNetUpdateFrequency(100.f);
	
	AttackComponent = CreateDefaultSubobject<UMCOAttackComponent>(TEXT("Attack Component"));
	
	// Ignore Camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	
	// Disable Mesh collision
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

