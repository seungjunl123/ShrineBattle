// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "DynastyGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FDynastyGameLevelSet
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
    FGameplayTag LevelTag;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> Level;

    bool IsValid() const
    {
        return LevelTag.IsValid() && !Level.IsNull();
    }


};
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDynastyGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
    virtual void OnPreLoadMap(const FString& MapName);
    virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FDynastyGameLevelSet> GameLevelSets;


public:
    virtual void Init() override;

    UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
    TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag);
};
