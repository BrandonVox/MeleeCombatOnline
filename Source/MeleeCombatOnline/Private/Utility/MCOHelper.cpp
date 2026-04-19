// Copyright Brandon Vox.


#include "Utility/MCOHelper.h"

bool UMCOHelper::HasAuthority(const AActor* InActor)
{
	if (InActor == nullptr)
	{
		return false;
	}
	
	return InActor->HasAuthority();
}
