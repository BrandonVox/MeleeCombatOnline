// Copyright Brandon Vox.


#include "Component/WidgetComponent_Overhead.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/Attribute/AttributeSet_Base.h"
#include "UI/Widget_Overhead.h"

void UWidgetComponent_Overhead::BeginPlay()
{
	Super::BeginPlay();

	// get attribute value from ASC
	// get ASC
	// Update Overhead Widget
	// Listen when attribute value changed

	IAbilitySystemInterface* OwnerASI = GetOwner<IAbilitySystemInterface>();

	if (OwnerASI == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* OwnerASC = OwnerASI->GetAbilitySystemComponent();

	if (OwnerASC == nullptr)
	{
		return;
	}

	bool bFound;
	CachedHealth = OwnerASC->GetGameplayAttributeValue(UAttributeSet_Base::GetHealthAttribute(), bFound);
	CachedMaxHealth = OwnerASC->GetGameplayAttributeValue(UAttributeSet_Base::GetMaxHealthAttribute(), bFound);

	UpdateHealthBar();
}

void UWidgetComponent_Overhead::UpdateHealthBar()
{
	UWidget_Overhead* MyWidget_Overhead = Cast<UWidget_Overhead>(GetUserWidgetObject());
	
	if (MyWidget_Overhead == nullptr)
	{
		return;
	}
	
	MyWidget_Overhead->UpdateHealthBar(CachedHealth, CachedMaxHealth);
}
