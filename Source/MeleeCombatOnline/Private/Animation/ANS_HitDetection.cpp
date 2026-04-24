// Copyright Brandon Vox.


#include "Animation/ANS_HitDetection.h"

UANS_HitDetection::UANS_HitDetection()
{
#if WITH_EDITOR
	NotifyColor = FColor::Red;
#endif 
}

FString UANS_HitDetection::GetNotifyName_Implementation() const
{
	return FString(TEXT("HitDetection"));
}
