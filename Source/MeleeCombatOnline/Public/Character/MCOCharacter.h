// Copyright Brandon Vox.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCOCharacter.generated.h"

UCLASS()
class MELEECOMBATONLINE_API AMCOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMCOCharacter();

protected:
	virtual void BeginPlay() override;
};
