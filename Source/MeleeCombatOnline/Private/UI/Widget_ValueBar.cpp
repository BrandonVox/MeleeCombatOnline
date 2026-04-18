// Copyright Brandon Vox.


#include "UI/Widget_ValueBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWidget_ValueBar::UpdateValue(float NewValue, float NewMaxValue)
{
	if (NewMaxValue == 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Value can't be 0!"));
		return;
	}

	Bar_Value->SetPercent(NewValue / NewMaxValue);


	FText NewText = FText::Format(FTextFormat::FromString("{0}/{1}"), NewValue, NewMaxValue);
	Text_Value->SetText(NewText);
}
