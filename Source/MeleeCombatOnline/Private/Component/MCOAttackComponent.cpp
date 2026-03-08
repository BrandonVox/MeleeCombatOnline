// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogTraceHit, Log, All);

UMCOAttackComponent::UMCOAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UMCOAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttackComponent, CurrentState, COND_None, REPNOTIFY_OnChanged);
}

void UMCOAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMCOAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMCOAttackComponent::LocalInputPressed()
{
	// Listen Server
	if (HasAuthority(GetOwner()))
	{
		TryAttack();
		return;
	}

	// Client
	Server_TryAttack();
}

void UMCOAttackComponent::Server_TryAttack_Implementation()
{
	TryAttack();
}

void UMCOAttackComponent::TryAttack()
{
	if (!CanAttack())
	{
		return;
	}

	if (HasAuthority(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bIsAttacking = true;
		CurrentState.bComboWindowOpened = false;
		++CurrentState.AttackCount;

		HandleCurrentStateChanged(OldState);
	}
}

bool UMCOAttackComponent::CanAttack() const
{
	if (!CurrentState.bIsAttacking)
	{
		return true;
	}

	if (CurrentState.bComboWindowOpened)
	{
		return true;
	}

	return false;
}

void UMCOAttackComponent::HandleCurrentStateChanged(const FAttackState& OldState)
{
	if (CurrentState.bIsAttacking && CurrentState.AttackCount > OldState.AttackCount)
	{
		if (ACharacter* MyCharacter = GetOwnerCharacter())
		{
			MyCharacter->PlayAnimMontage(GetAttackMontage(CurrentState.AttackCount, CurrentState.IndexOffset));
		}
	}
}

UAnimMontage* UMCOAttackComponent::GetAttackMontage(const uint16 InAttackCount, const uint16 IndexOffset) const
{
	if (InAttackCount == 0)
	{
		return nullptr;
	}

	if (Montages_Attack.IsEmpty())
	{
		return nullptr;
	}

	const uint16 CalculatedIndex = (InAttackCount - IndexOffset) % Montages_Attack.Num();

	return Montages_Attack[CalculatedIndex];
}

void UMCOAttackComponent::BeginHitDetection()
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Hit Detection"));

	ACharacter* MyCharacter = GetOwnerCharacter();

	if (MyCharacter == nullptr)
	{
		return;
	}

	if (MyCharacter->GetMesh() == nullptr)
	{
		return;
	}

	OldTraceStart = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_Start);
	OldTraceEnd = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_End);
}

void UMCOAttackComponent::EndHitDetection()
{
	// UE_LOG(LogTemp, Warning, TEXT("Frame Count = %d"), FrameCount);
}

void UMCOAttackComponent::TickHitDetection()
{
	ACharacter* MyCharacter = GetOwnerCharacter();

	if (MyCharacter == nullptr)
	{
		return;
	}

	if (MyCharacter->GetMesh() == nullptr)
	{
		return;
	}

	const FVector TraceStart = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_Start);
	const FVector TraceEnd = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_End);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	EDrawDebugTrace::Type DrawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (!bDrawDebugClient)
	{
		if (!HasAuthority(GetOwner()))
		{
			DrawDebugType = EDrawDebugTrace::None;
		}
	}

	FLinearColor TraceColor = FLinearColor::Red;

	if (HasAuthority(MyCharacter))
	{
		TraceColor = FLinearColor::Blue;
	}

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		this,
		TraceStart,
		TraceEnd,
		TraceRadius,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		HitResults,
		false,
		TraceColor,
		FLinearColor::Green,
		TraceDrawTime
	);

	// we need Hit Results here
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* VictimActor = HitResult.GetActor();
		if (VictimActor == nullptr)
		{
			continue;
		}

		if (HitActorsThisSwing.Contains(VictimActor))
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *VictimActor->GetName());
		HitActorsThisSwing.Add(VictimActor);
	}

	OldTraceStart = TraceStart;
	OldTraceEnd = TraceEnd;
}

void UMCOAttackComponent::DoTrace()
{
}

void UMCOAttackComponent::OpenComboWindow()
{
	if (HasAuthority(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bComboWindowOpened = true;

		HandleCurrentStateChanged(OldState);
	}
}

void UMCOAttackComponent::EndAttack()
{
	if (HasAuthority(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bIsAttacking = false;
		CurrentState.bComboWindowOpened = false;
		// Reset Attack Index -> 0
		CurrentState.IndexOffset = CurrentState.AttackCount + 1;

		HandleCurrentStateChanged(OldState);
	}
}

bool UMCOAttackComponent::HasAuthority(const AActor* InActor)
{
	return InActor && InActor->HasAuthority();
}

void UMCOAttackComponent::OnRep_CurrentState(const FAttackState& OldState)
{
	HandleCurrentStateChanged(OldState);
}

ACharacter* UMCOAttackComponent::GetOwnerCharacter()
{
	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = GetOwner<ACharacter>();
	}

	return OwnerCharacter;
}
