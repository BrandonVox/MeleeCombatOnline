// Copyright Brandon Vox.


#include "GAS/Component/MCO_ASC.h"

void UMCO_ASC::GiveDefaultAbilities()
{
	GiveAbility(FGameplayAbilitySpec(Class_GA_BasicAttack, 1, 1));
}
