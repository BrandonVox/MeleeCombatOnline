// Copyright Brandon Vox.


#include "Component/MCOAttackComponent.h"

#include "EditorActorFolders.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTraceHit, Log, All);

UMCOAttackComponent::UMCOAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UMCOAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttackComponent, CurrentState, COND_SkipOwner, REPNOTIFY_OnChanged);
}

void UMCOAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ACharacter* MyOwnerCharacter = GetOwnerCharacter();
	// FVector OldStart = MyOwnerCharacter->GetActorLocation();
	// FVector OldEnd = MyOwnerCharacter->GetActorLocation() + (MyOwnerCharacter->GetActorForwardVector() * 500.f);
	//
	// FVector CurrentStart = OldStart + MyOwnerCharacter->GetActorRightVector() * 84.f;
	// FVector CurrentEnd = OldEnd + MyOwnerCharacter->GetActorRightVector() * 84.f;
	// TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(MyOwnerCharacter);
	//
	//
	// float CapsuleLength = FVector::Distance(CurrentStart, CurrentEnd);
	// FillTraceGap(CurrentStart, CurrentEnd, OldStart, OldEnd, ActorsToIgnore, FLinearColor::Yellow, CapsuleLength);
}

void UMCOAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMCOAttackComponent::LocalInputPressed()
{
	TryAttack();
}

void UMCOAttackComponent::TryAttack()
{
	if (!CanAttack())
	{
		return;
	}

	if (HasAuthorityOrClientCanPredict(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bIsAttacking = true;
		CurrentState.bComboWindowOpened = false;
		++CurrentState.AttackCount;

		if (LocalRoleIsAutonomousProxy(GetOwner()))
		{
			Server_ClientIsAboutToAttack(OldState);
		}
		HandleCurrentStateChanged(OldState);
	}
}

// Server
void UMCOAttackComponent::Server_ClientIsAboutToAttack_Implementation(const FAttackState& OldClientState)
{
	if (!CanAttack())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server denied attack!!!"));
		FAttackState GoodState = OldClientState;
		GoodState.AttackCount = CurrentState.AttackCount;
		GoodState.IndexOffset = CurrentState.IndexOffset;
		Client_ServerDeniedAttack(GoodState);
		return;
	}

	FAttackState OldState = CurrentState;

	CurrentState.bIsAttacking = true;
	CurrentState.bComboWindowOpened = false;
	++CurrentState.AttackCount;

	Client_ConfirmAttack(CurrentState);

	HandleCurrentStateChanged(OldState);
}

// Client
void UMCOAttackComponent::Client_ConfirmAttack_Implementation(const FAttackState& ServerAttackState)
{
	// idealy, client & server should identical here
	// if we check server attack state, can attack -->>> false
	// but if we can attack in client, means client go to next phase of animation!!!!!!!!!!!!!!!!!!!!!!!!!

	if (CanAttack()) // client already go to next phase
	{
		return;
	}

	if (CurrentState != ServerAttackState)
	{
		FAttackState OldState = CurrentState;
		CurrentState = ServerAttackState;
		HandleCurrentStateChanged(OldState);
	}
}

void UMCOAttackComponent::Client_ServerDeniedAttack_Implementation(const FAttackState& OldClientState)
{
	if (ACharacter* MyOwnerCharacter = GetOwnerCharacter())
	{
		MyOwnerCharacter->StopAnimMontage();
	}

	CurrentState = OldClientState;
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
		if (ACharacter* MyOwnerCharacter = GetOwnerCharacter())
		{
			MyOwnerCharacter->PlayAnimMontage(GetAttackMontage(CurrentState.AttackCount, CurrentState.IndexOffset));

			if (!HasAuthority(GetOwner()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Play Montage, Attack Count: %d"), CurrentState.AttackCount);
			}
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
	// UE_LOG(LogTemp, Warning, TEXT("Begin Hit Detection"));

	HitActorsThisSwing.Empty();

	ACharacter* MyOwnerCharacter = GetOwnerCharacter();
	OldTraceStart = GetSocketLocation(MyOwnerCharacter, TraceSocketName_Start);
	OldTraceEnd = GetSocketLocation(MyOwnerCharacter, TraceSocketName_End);
}

void UMCOAttackComponent::EndHitDetection()
{
	// UE_LOG(LogTemp, Warning, TEXT("Frame Count = %d"), FrameCount);
	HitActorsThisSwing.Empty();

	ACharacter* MyOwnerCharacter = GetOwnerCharacter();
	OldTraceStart = GetSocketLocation(MyOwnerCharacter, TraceSocketName_Start);
	OldTraceEnd = GetSocketLocation(MyOwnerCharacter, TraceSocketName_End);
}

void UMCOAttackComponent::TickHitDetection()
{
	ACharacter* MyOwnerCharacter = GetOwnerCharacter();

	FVector TraceStart = GetSocketLocation(MyOwnerCharacter, TraceSocketName_Start);
	FVector TraceEnd = GetSocketLocation(MyOwnerCharacter, TraceSocketName_End);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MyOwnerCharacter);

	FLinearColor TraceColor = FLinearColor::Red;

	if (HasAuthority(MyOwnerCharacter))
	{
		TraceColor = FLinearColor::Blue;
	}

	TraceAndProcessHitResults(TraceStart, TraceEnd, ActorsToIgnore, TraceColor);


	float CapsuleLength = FVector::Distance(TraceStart, TraceEnd);
	FillTraceGap(TraceStart, TraceEnd, OldTraceStart, OldTraceEnd, ActorsToIgnore, FLinearColor::Yellow, CapsuleLength);

	OldTraceStart = TraceStart;
	OldTraceEnd = TraceEnd;
}

void UMCOAttackComponent::TraceAndProcessHitResults(FVector TraceStart,
                                                    FVector TraceEnd,
                                                    const TArray<AActor*>& ActorsToIgnore,
                                                    FLinearColor TraceColor)
{
	// Trace
	EDrawDebugTrace::Type DrawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (!bDrawDebugClient)
	{
		if (!HasAuthority(GetOwner()))
		{
			DrawDebugType = EDrawDebugTrace::None;
		}
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

	// Process Hit Results
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

		if (HasAuthority(GetOwner()))
		{
			// UE_LOG(LogTemp, Warning, TEXT("Server: %s"), *VictimActor->GetName());
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("Client: %s"), *VictimActor->GetName());
		}

		HitActorsThisSwing.Add(VictimActor);
	}
}

void UMCOAttackComponent::FillTraceGap(FVector CurrentStart,
                                       FVector CurrentEnd,
                                       FVector OldStart,
                                       FVector OldEnd,
                                       const TArray<AActor*>& ActorsToIgnore,
                                       FLinearColor TraceColor,
                                       float CapsuleLength)
{
	float TwoEndDistance = FVector::Distance(OldEnd, CurrentEnd);
	FVector TwoEndDirection = (CurrentEnd - OldEnd).GetSafeNormal();

	// Fill 1 trace each 20 unit
	int32 FillNumber = FMath::TruncToInt32(TwoEndDistance / TraceRadius);

	float TwoStartDistance = FVector::Distance(OldStart, CurrentStart);
	FVector TwoStartDirection = (CurrentStart - OldStart).GetSafeNormal();

	float StartStep = (TwoStartDistance / FillNumber);

	// UE_LOG(LogTemp, Warning, TEXT("Fillers: %d"), FillNumber);

	// fill number = 4
	// 1 2 3 4
	for (int32 i = 1; i <= FillNumber; ++i)
	{
		FVector FillerEnd = OldEnd + (TwoEndDirection * TraceRadius * i);
		FVector FillerStart = OldStart + (TwoStartDirection * StartStep * i);

		// Direction from Start -> End
		FVector CapsuleDirection = (FillerEnd - FillerStart).GetSafeNormal();

		FillerEnd = FillerStart + (CapsuleDirection * CapsuleLength);

		TraceAndProcessHitResults(FillerStart, FillerEnd, ActorsToIgnore, TraceColor);
	}
}


void UMCOAttackComponent::OpenComboWindow()
{
	if (HasAuthorityOrClientCanPredict(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bComboWindowOpened = true;

		HandleCurrentStateChanged(OldState);
	}
}


void UMCOAttackComponent::EndAttack()
{
	if (HasAuthorityOrClientCanPredict(GetOwner()))
	{
		FAttackState OldState = CurrentState;

		CurrentState.bIsAttacking = false;
		CurrentState.bComboWindowOpened = false;
		// Reset Attack Index -> 0
		CurrentState.IndexOffset = CurrentState.AttackCount + 1;

		HandleCurrentStateChanged(OldState);
	}
}


void UMCOAttackComponent::OnRep_CurrentState(const FAttackState& OldState)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_CurrentState"));
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

FVector UMCOAttackComponent::GetSocketLocation(ACharacter* InCharacter, FName InSocketName)
{
	if (InCharacter && InCharacter->GetMesh())
	{
		return InCharacter->GetMesh()->GetSocketLocation(InSocketName);
	}

	return FVector::ZeroVector;
}


bool UMCOAttackComponent::HasAuthority(const AActor* InActor)
{
	return InActor && InActor->HasAuthority();
}

bool UMCOAttackComponent::LocalRoleIsAutonomousProxy(const AActor* InActor)
{
	return InActor && InActor->GetLocalRole() == ROLE_AutonomousProxy;
}

bool UMCOAttackComponent::HasAuthorityOrClientCanPredict(const AActor* InActor)
{
	return HasAuthority(InActor) || LocalRoleIsAutonomousProxy(InActor);
}
