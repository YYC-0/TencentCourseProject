// Copyright Epic Games, Inc. All Rights Reserved.

#include "TencentCourseProjectGameMode.h"
#include "TencentCourseProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATencentCourseProjectGameMode::ATencentCourseProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
