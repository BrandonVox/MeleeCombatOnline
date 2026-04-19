// Copyright Brandon Vox.


#include "GAS/Component/MCO_ASC.h"

void UMCO_ASC::GiveDefaultAbilities()
{
	GiveAbility(FGameplayAbilitySpec(Class_GA_BasicAttack, 1, 1));
}

void UMCO_ASC::ApplyInitialEffects()
{
	if (GetOwner() == nullptr)
	{
		return;
	}
	
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	for (const TSubclassOf<UGameplayEffect>& Class_Effect : Classes_GE_Initial)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(Class_Effect, 1.f, MakeEffectContext());
		FGameplayEffectSpec* EffectSpec = EffectSpecHandle.Data.Get();
		ApplyGameplayEffectSpecToSelf(*EffectSpec); // Spec
	}
}
