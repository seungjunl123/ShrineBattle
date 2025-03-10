// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/StartUpData/DataAsset_StartUpBase.h"
#include "GameplayTagContainer.h"
#include "DataAsset_KwangStartUpData.generated.h"

struct FKwangAbilitySet;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDataAsset_KwangStartUpData : public UDataAsset_StartUpBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UDynastyAbilitySystemComponent* InASCToGive, int32 AbilityLevel = 1) override;

private:

	UPROPERTY(EditDefaultsOnly, Category="StartUpData", meta=(Categories="InputTag"))
	TArray<FKwangAbilitySet> KwangStartupAbilitySets;
};
