// Copyright Brandon Vox.


#include "Animation/MCOAnimNotify_OpenComboWindow.h"

UMCOAnimNotify_OpenComboWindow::UMCOAnimNotify_OpenComboWindow()
{
#if WITH_EDITOR
	NotifyColor = FColor::Green;
#endif
}

FString UMCOAnimNotify_OpenComboWindow::GetNotifyName_Implementation() const
{
	return FString(TEXT("Combo"));
}
