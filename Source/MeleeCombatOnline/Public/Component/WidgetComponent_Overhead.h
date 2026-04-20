// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WidgetComponent_Overhead.generated.h"

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

private: // Property
	float CachedHealth = 0.f;
	float CachedMaxHealth = 0.f;
};
