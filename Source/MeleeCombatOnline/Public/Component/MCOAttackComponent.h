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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	void LocalInputPressed();
	void EndAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private: // Function
	UFUNCTION(Server, Reliable)
	void Server_TryAttack();
	void TryAttack();
	void OnSet_bIsAttacking();
	void IncreaseAttackIndex();
	static bool HasAuthority(const AActor* InActor);
	
	UFUNCTION()
	void OnRep_bIsAttacking(bool bOldValue);

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Attack")
	TArray<TObjectPtr<UAnimMontage>> Montages_Attack;
	
	UPROPERTY(ReplicatedUsing = OnRep_bIsAttacking)
	bool bIsAttacking = false;
	
	
	uint8 AttackIndex = 0;
};
