// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCOCharacter.generated.h"

class UMCO_ASC;
class UMCOAttackComponent;

UCLASS(Abstract)
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
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMCO_ASC> MCO_ASC;
};