// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MCOCharacter.generated.h"


class UAttributeSet_Base;
class UWidgetComponent_Overhead;
class UMCO_ASC;

UCLASS()
class MELEECOMBATONLINE_API AMCOCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMCOCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;

protected: // Property
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMCO_ASC> MCO_ASC;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent_Overhead> WidgetComponent_Overhead;
	
	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet_Base> AttributeSet_Base;
};
