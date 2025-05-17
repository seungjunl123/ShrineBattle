// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DynastyTypes/DynastyEnumTypes.h"
#include "DynastySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDynastySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	EDynastyGameDifficulty SavedCurrentGameDifficulty;
	
};
