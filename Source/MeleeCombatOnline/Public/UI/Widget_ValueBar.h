// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ValueBar.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UWidget_ValueBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateValue(float NewValue, float NewMaxValue);
	
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_Value;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Value;
	
};
