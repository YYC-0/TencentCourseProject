// Copyright Epic Games, Inc. All Rights Reserved.

#include "TencentCourseProjectGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"

ATencentCourseProjectGameMode::ATencentCourseProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Ue4ASP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//static ConstructorHelpers::FClassFinder<AHUD> GameHUD(TEXT("Blueprint'/Game/Blueprints/HUD'"));
	//if (GameHUD.Succeeded())
	//{
	//	HUDClass = GameHUD.Class;
	//}
}
