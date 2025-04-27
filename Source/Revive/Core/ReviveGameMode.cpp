// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReviveGameMode.h"
#include "Revive/Characters/ReviveCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReviveGameMode::AReviveGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/RoleSelect_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//设置默认控制器类
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/Player/Player_PC"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
