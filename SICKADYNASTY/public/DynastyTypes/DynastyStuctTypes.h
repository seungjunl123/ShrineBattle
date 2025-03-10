// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "DynastyStuctTypes.generated.h"

class UKwangGameplayAbility;

USTRUCT(BlueprintType)
struct FKwangAbilitySet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="InputTag"))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UKwangGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};
