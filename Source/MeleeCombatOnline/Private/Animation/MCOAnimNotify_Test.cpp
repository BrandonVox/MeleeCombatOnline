// Copyright Brandon Vox.


#include "Animation/MCOAnimNotify_Test.h"

#include "Kismet/GameplayStatics.h"

UMCOAnimNotify_Test::UMCOAnimNotify_Test()
{
	bIsNativeBranchingPoint = true;
}

void UMCOAnimNotify_Test::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	
	if (!MeshComp)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("BranchingPointNotify Test: %f"), UGameplayStatics::GetWorldDeltaSeconds(MeshComp->GetOwner()));
}

void UMCOAnimNotify_Test::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Notify Test: %f"), UGameplayStatics::GetWorldDeltaSeconds(MeshComp->GetOwner()));
}
