// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()



public:
	UPawnCombatComponent();

	virtual void OnHitTargetActor(AActor* HitActor);

	void DelegateBindingToActor();
};
