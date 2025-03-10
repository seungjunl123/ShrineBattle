// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "PlayerUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UPlayerUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentManaChanged;
};
