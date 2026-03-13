// Copyright Brandon Vox.


#include "Character/MCOCharacter.h"

#include "Component/MCOAttackComponent.h"
#include "Components/CapsuleComponent.h"

AMCOCharacter::AMCOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetNetUpdateFrequency(64.f);
	
	AttackComponent = CreateDefaultSubobject<UMCOAttackComponent>(TEXT("Attack Component"));
	
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

