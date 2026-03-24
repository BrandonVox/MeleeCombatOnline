// Copyright Brandon Vox.


#include "GameMode/MCOGameModeBase.h"

void AMCOGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
#if ENABLE_VISUAL_LOG
	FVisualLogger::Get().SetIsRecording(true);
#endif
}

void AMCOGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
#if ENABLE_VISUAL_LOG
	FVisualLogger::Get().SetIsRecording(false);
#endif
}
