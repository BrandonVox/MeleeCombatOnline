// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCOCharacter.generated.h"

class UMCOAttackComponent;

UCLASS()
class MELEECOMBATONLINE_API AMCOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMCOCharacter();

protected:
	virtual void BeginPlay() override;

protected: // Property
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMCOAttackComponent> AttackComponent;
};
