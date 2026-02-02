// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "MCOCharacter.h"
#include "MCOCharacter_Player.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class MELEECOMBATONLINE_API AMCOCharacter_Player : public AMCOCharacter
{
	GENERATED_BODY()

public: // Function
	AMCOCharacter_Player();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

protected:
	virtual void BeginPlay() override;
	
private: // Function
	void SetupInputMappingContext() const;
	
private: // Property
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category= "MCO Settings | Input")
	TObjectPtr<UInputMappingContext> MyInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category= "MCO Settings | Input")
	TObjectPtr<UInputAction> InputAction_Jump;
	

};
