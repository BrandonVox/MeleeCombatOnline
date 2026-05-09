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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	ATTRIBUTE_ACCESSORS_BASIC(UAttributeSet_Base, Health)
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeSet_Base, MaxHealth)

private: // Function
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

private: // Property
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
};
