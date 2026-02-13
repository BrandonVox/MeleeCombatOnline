// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MCOAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATONLINE_API UMCOAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	// Function
	// Sets default values for this component's properties
	UMCOAttackComponent();
	void LocalInputPressed();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
