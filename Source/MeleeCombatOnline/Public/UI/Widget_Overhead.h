// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Overhead.generated.h"

class UWidget_ValueBar;
/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UWidget_Overhead : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealthBar(float NewValue, float NewMaxValue);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget_ValueBar> HealthBar;
};
