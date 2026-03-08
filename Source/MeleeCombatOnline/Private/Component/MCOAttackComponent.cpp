// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

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
	FrameCount = 0;
	EndLocationHistory.Empty();

	if (const ACharacter* MyCharacter = GetOwnerCharacter())
	{
		PrevEndLocation = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_End);
	}
}

void UMCOAttackComponent::EndHitDetection()
{
	UE_LOG(LogTemp, Warning, TEXT("Frame Count = %d"), FrameCount);

#if ENABLE_VISUAL_LOG
	if (FVisualLogger::Get().IsRecording())
	{
		for (int32 i = 0; i < EndLocationHistory.Num() - 1; ++i)
		{
			UE_VLOG_SEGMENT
			(
				GetOwner(),
				"LogTraceHit",
				Verbose,
				EndLocationHistory[i],
				EndLocationHistory[i+1],
				FColor::Blue,
				TEXT("")
			);
		}
	}
#endif

	if (const ACharacter* MyCharacter = GetOwnerCharacter())
	{
		PrevEndLocation = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_End);
	}
	FrameCount = 0;
	EndLocationHistory.Empty();
}

void UMCOAttackComponent::TickHitDetection()
{
	++FrameCount;

	ACharacter* MyCharacter = GetOwnerCharacter();

	if (MyCharacter == nullptr)
	{
		return;
	}

	if (MyCharacter->GetMesh() == nullptr)
	{
		return;
	}

	const FVector TraceLocation_Start = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_Start);
	const FVector TraceLocation_End = MyCharacter->GetMesh()->GetSocketLocation(TraceSocketName_End);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	EDrawDebugTrace::Type DrawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (!HasAuthority(GetOwner()))
	{
		// DrawDebugType = EDrawDebugTrace::None;
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
		TraceLocation_Start,
		TraceLocation_End,
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

#if ENABLE_VISUAL_LOG
	if (FVisualLogger::Get().IsRecording())
	{
		UE_VLOG_LOCATION
		(
			GetOwner(),
			"LogTraceHit",
			Display,
			TraceLocation_End,
			5,
			FColor::Red,
			TEXT("Trace Hit: %s"),
			*TraceLocation_End.ToString()
		);
	}
#endif

	EndLocationHistory.Add(TraceLocation_End);
	PrevEndLocation = TraceLocation_End;
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
