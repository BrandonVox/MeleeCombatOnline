// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MCOAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MELEECOMBATONLINE_API UMCOAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public: // Function
	// Sets default values for this component's properties
	UMCOAttackComponent();
	void LocalInputPressed();
	void EndAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private: // Function
	void TryAttack();
	void OnSet_bIsAttacking();
	void IncreaseAttackIndex();

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Attack")
	TArray<TObjectPtr<UAnimMontage>> Montages_Attack;
	
	bool bIsAttacking = false;
	uint8 AttackIndex = 0;
};
