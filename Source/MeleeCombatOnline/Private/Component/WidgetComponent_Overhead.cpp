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

	// Bind Delegate
	OwnerASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Base::GetHealthAttribute())
	        .AddUObject(this, &UWidgetComponent_Overhead::ValueChanged_Health);

	OwnerASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Base::GetMaxHealthAttribute())
	        .AddUObject(this, &UWidgetComponent_Overhead::ValueChanged_MaxHealth);
}

void UWidgetComponent_Overhead::UpdateHealthBar()
{
	UWidget_Overhead* MyWidget_Overhead = GetWidget_Overhead();

	if (MyWidget_Overhead == nullptr)
	{
		return;
	}

	MyWidget_Overhead->UpdateHealthBar(CachedHealth, CachedMaxHealth);
}

UWidget_Overhead* UWidgetComponent_Overhead::GetWidget_Overhead()
{
	if (CachedWidget_Overhead == nullptr)
	{
		CachedWidget_Overhead = Cast<UWidget_Overhead>(GetUserWidgetObject());
	}

	return CachedWidget_Overhead;
}

void UWidgetComponent_Overhead::ValueChanged_Health(const FOnAttributeChangeData& AttributeChangeData)
{
	CachedHealth = AttributeChangeData.NewValue;
	UpdateHealthBar();
}

void UWidgetComponent_Overhead::ValueChanged_MaxHealth(const FOnAttributeChangeData& AttributeChangeData)
{
	CachedMaxHealth = AttributeChangeData.NewValue;
	UpdateHealthBar();
}
