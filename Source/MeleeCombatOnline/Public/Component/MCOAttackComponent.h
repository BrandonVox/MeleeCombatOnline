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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void LocalInputPressed();

	void BeginHitDetection();
	void EndHitDetection();
	void TickHitDetection();

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

	ACharacter* GetOwnerCharacter();

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Attack")
	TArray<TObjectPtr<UAnimMontage>> Montages_Attack;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	FAttackState CurrentState;

	int32 FrameCount = 0;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	FName TraceSocketName_Start;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	FName TraceSocketName_End;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	float TraceRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	bool bDrawDebugTrace = true;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	float TraceDrawTime = 5.f;
	
	UPROPERTY()
	TArray<FVector> EndLocationHistory;
	
	UPROPERTY()
	FVector PrevEndLocation;
};
