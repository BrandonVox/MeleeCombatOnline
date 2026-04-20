// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WidgetComponent_Overhead.generated.h"

class UWidget_Overhead;
/**
 * 
 */
UCLASS()
class MELEECOMBATONLINE_API UWidgetComponent_Overhead : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
private: // Function
	void UpdateHealthBar();
	
	UWidget_Overhead* GetWidget_Overhead();

private: // Property
	float CachedHealth = 0.f;
	float CachedMaxHealth = 0.f;
	
	UPROPERTY()
	TObjectPtr<UWidget_Overhead> CachedWidget_Overhead;
};
