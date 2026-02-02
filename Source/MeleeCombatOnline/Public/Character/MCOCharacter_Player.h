// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "MCOCharacter.h"
#include "MCOCharacter_Player.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MELEECOMBATONLINE_API AMCOCharacter_Player : public AMCOCharacter
{
	GENERATED_BODY()

public:
	AMCOCharacter_Player();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
private: // Property
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;
	

};
