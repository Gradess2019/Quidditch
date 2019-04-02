// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETeamSide : uint8
{
	None,
	Blue,
	Red
};

UENUM(BlueprintType)
enum class ERoleInTeam : uint8
{
	Chaser,
	Beater,
	Seeker,
	Keeper
};

UENUM(BlueprintType)
enum class EHand : uint8
{
	Right,
	Left
};