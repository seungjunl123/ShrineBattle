// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, TotalCooldownTime, float, RemainingCooldownTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoneInteractedDelegate, bool, bShouldDisplayInputKey);

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UPlayerUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentManaChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnStoneInteractedDelegate OnStoneInteracted;
};
