// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MCOAttackComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttackState
{
	GENERATED_BODY()

	bool operator==(FAttackState Other) const
	{
		return bIsAttacking == Other.bIsAttacking
			&& bComboWindowOpened == Other.bComboWindowOpened
			&& AttackCount == Other.AttackCount
			&& IndexOffset == Other.IndexOffset;
	}

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
	void TryAttack();
	bool CanAttack() const;
	void HandleCurrentStateChanged(const FAttackState& OldState);
	UAnimMontage* GetAttackMontage(const uint16 InAttackCount, const uint16 IndexOffset) const;

	UFUNCTION()
	void OnRep_CurrentState(const FAttackState& OldState);

	ACharacter* GetOwnerCharacter();


	APawn* GetOwnerPawn();

	void TraceAndProcessHitResults(FVector TraceStart,
	                               FVector TraceEnd,
	                               const TArray<AActor*>& ActorsToIgnore,
	                               FLinearColor TraceColor);

	static FVector GetSocketLocation(ACharacter* InCharacter, FName InSocketName);

	void FillTraceGap(FVector CurrentStart,
	                  FVector CurrentEnd,
	                  FVector OldStart,
	                  FVector OldEnd,
	                  const TArray<AActor*>& ActorsToIgnore,
	                  FLinearColor TraceColor,
	                  float CapsuleLength);

	UFUNCTION(Server, Reliable)
	void Server_ClientIsAboutToAttack(FAttackState ClientPredictedState);

	UFUNCTION(Client, Reliable)
	void Client_ServerDeniedAttack();

	static bool HasAuthority(const AActor* InActor);
	static bool LocalRoleIsAutonomousProxy(const AActor* InActor);
	static bool HasAuthorityOrClientCanPredict(const AActor* InActor);
	static bool IsLocallyControlled(const APawn* InPawn);
	bool IsLocallyControlled();

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Attack")
	TArray<TObjectPtr<UAnimMontage>> Montages_Attack;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	FAttackState CurrentState;
	
	UPROPERTY()
	FAttackState OldState_CanAttack;
	
	int32 FrameCount = 0;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	FName TraceSocketName_Start;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	FName TraceSocketName_End;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta=(ClampMin = 10.f, ClampMax = 50.f))
	float TraceRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	bool bDrawDebugTrace = true;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	float TraceDrawTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection", meta = (EditCondition = "bDrawDebugTrace"))
	bool bDrawDebugClient = true;

	UPROPERTY(EditDefaultsOnly, Category = "MCO Settings | Hit Detection")
	float BaseDamage = 20.f;

	FVector OldTraceStart; // Last Frame
	FVector OldTraceEnd; // Last Frame

	UPROPERTY()
	TSet<AActor*> HitActorsThisSwing;
};
