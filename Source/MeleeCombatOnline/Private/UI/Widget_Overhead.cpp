// Copyright Brandon Vox.


#include "UI/Widget_Overhead.h"

#include "UI/Widget_ValueBar.h"

void UWidget_Overhead::UpdateHealthBar(float NewValue, float NewMaxValue)
{
	HealthBar->UpdateValue(NewValue, NewMaxValue);
}
