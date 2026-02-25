// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MCOAttackComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttackState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bIsAttacking = false;
	
	UPROPERTY(EditAnywhere)
	bool bComboWindowOpened = false;

	UPROPERTY(EditAnywhere)
	uint16 AttackCount = 0;
	
	UPROPERTY(EditAnywhere)
	uint16 IndexOffset = 1;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MELEECOMBATONLINE_API UMCOAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public: // Function
	UMCOAttackComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void LocalInputPressed();
	void OpenComboWindow();
	void EndAttack();

protected:
	virtual void BeginPlay() override;

private: // Function
	UFUNCTION(Server, Reliable)
	void Server_TryAttack();
	void TryAttack();
	bool CanAttack() const;
	void HandleCurrentStateChanged(const FAttackState& OldState);
	UAnimMontage* GetAttackMontage(const uint16 InAttackCount, const uint16 IndexOffset) const;
	static bool HasAuthority(const AActor* InActor);
	
	UFUNCTION()
	void OnRep_CurrentState(const FAttackState& OldState);

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Attack")
	TArray<TObjectPtr<UAnimMontage>> Montages_Attack;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	FAttackState CurrentState;
};
