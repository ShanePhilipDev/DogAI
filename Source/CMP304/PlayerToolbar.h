// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "PlayerToolbar.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UPlayerToolbar : public UUserDefinedEnum
{
	GENERATED_BODY()

};

// The toolbar enum.
UENUM(BlueprintType)
enum ToolbarSelection
{
	Nothing UMETA(DisplayName = "Nothing"),
	Ball UMETA(DisplayName = "Ball"),
	Treat UMETA(DisplayName = "Treat"),
};