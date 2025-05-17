// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DynastyTypes/DynastyEnumTypes.h"
#include "DynastyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API ADynastyGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ADynastyGameModeBase();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
    EDynastyGameDifficulty CurrentGameDifficulty;

    FORCEINLINE EDynastyGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
};