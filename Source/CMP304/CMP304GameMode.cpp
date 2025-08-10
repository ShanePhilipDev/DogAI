// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP304GameMode.h"
#include "CMP304HUD.h"
#include "CMP304Character.h"
#include "UObject/ConstructorHelpers.h"

ACMP304GameMode::ACMP304GameMode()
	: Super()
{
	
	// Set default pawn.
	DefaultPawnClass = ACMP304Character::StaticClass();

	// use our custom HUD class
	HUDClass = ACMP304HUD::StaticClass();
}
