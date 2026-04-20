// Copyright Brandon Vox.


#include "GAS/Attribute/AttributeSet_Base.h"

#include "Net/UnrealNetwork.h"

void UAttributeSet_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Base, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Base, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UAttributeSet_Base::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Base, Health, OldValue);
}

void UAttributeSet_Base::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Base, MaxHealth, OldValue);
}
