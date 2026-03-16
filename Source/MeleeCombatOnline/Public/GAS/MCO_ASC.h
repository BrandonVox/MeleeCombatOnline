// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MCO_ASC.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UMCO_ASC : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void GiveDefaultAbilities();

private:
	UPROPERTY(EditDefaultsOnly, Category="MCO Settings | GAS")
	TSubclassOf<UGameplayAbility> Class_GA_BasicAttack;
};
