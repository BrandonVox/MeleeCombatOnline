// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttributeSet_Base.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UAttributeSet_Base : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeSet_Base, Health)
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeSet_Base, MaxHealth)
	
private:
	UPROPERTY()
	FGameplayAttributeData Health;
	
	UPROPERTY()
	FGameplayAttributeData MaxHealth;
	
};
