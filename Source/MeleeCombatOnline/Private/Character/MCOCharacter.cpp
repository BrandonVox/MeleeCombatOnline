// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/MCOAttackComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/MCO_ASC.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetNetUpdateFrequency(64.f);

	AttackComponent = CreateDefaultSubobject<UMCOAttackComponent>(TEXT("Attack Component"));
	
	MCO_ASC = CreateDefaultSubobject<UMCO_ASC>(TEXT("MCO_ASC"));
	MCO_ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Ignore Camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Disable Mesh collision
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Anim Tick Option
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();
}
