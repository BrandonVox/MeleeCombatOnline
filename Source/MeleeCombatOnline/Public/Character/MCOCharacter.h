// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MCOCharacter.generated.h"

class UMCO_ASC;
class UMCOAttackComponent;

UCLASS(Abstract)
class MELEECOMBATONLINE_API AMCOCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMCOCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

protected: // Property
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMCO_ASC> MCO_ASC;
};
