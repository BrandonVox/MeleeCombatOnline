// Copyright Brandon Vox.


#include "Character/MCOCharacter_Player.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMCOCharacter_Player::AMCOCharacter_Player()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMCOCharacter_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AMCOCharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(
			InputAction_Jump,
			ETriggerEvent::Triggered,
			this,
			&AMCOCharacter_Player::Input_Triggered_Jump
		);

		EnhancedInputComponent->BindAction(
			InputAction_Look,
			ETriggerEvent::Triggered,
			this,
			&AMCOCharacter_Player::Input_Triggered_Look
		);

		EnhancedInputComponent->BindAction(
			InputAction_Move,
			ETriggerEvent::Triggered,
			this,
			&AMCOCharacter_Player::Input_Triggered_Move
		);
	}
}

void AMCOCharacter_Player::PawnClientRestart()
{
	Super::PawnClientRestart();
	/*
	 * Player controller -> Pawn/Character
	 * Local Player -> Player Controller -> Pawn/Character
	 * Camera / Input bindings
	 * Client machines
	 * Listen server, 
	 * Add Input Mapping Context by Local Player
	 */

	SetupInputMappingContext();
}

void AMCOCharacter_Player::SetupInputMappingContext() const
{
	if (APlayerController* MyPlayerController = GetController<APlayerController>())
	{
		ULocalPlayer* MyLocalPlayer = MyPlayerController->GetLocalPlayer();

		UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyLocalPlayer);

		if (InputSubsystem)
		{
			InputSubsystem->RemoveMappingContext(MyInputMappingContext);
			InputSubsystem->AddMappingContext(MyInputMappingContext, 0);
		}
	}
}

void AMCOCharacter_Player::Input_Triggered_Jump()
{
	Jump();
	UE_LOG(LogTemp, Warning, TEXT("Input_Triggered_Jump"));
}

void AMCOCharacter_Player::Input_Triggered_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputValue2D = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(InputValue2D.X);
	AddControllerPitchInput(InputValue2D.Y);
}

void AMCOCharacter_Player::Input_Triggered_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputValue2D = InputActionValue.Get<FVector2D>().GetClampedToMaxSize(1.f);
	
	UE_LOG(LogTemp, Warning, TEXT("Input Mag = %f"), InputValue2D.Length());
	
	// input value X Y

	const FVector RightDirection = CameraComponent->GetRightVector();
	const FVector ForwardDirection = FVector::CrossProduct(RightDirection, FVector::UpVector).GetSafeNormal();
	
	
	AddMovementInput(RightDirection, InputValue2D.X);
	AddMovementInput(ForwardDirection, InputValue2D.Y);
}
