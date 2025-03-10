// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "DynastyTypes/DynastyStuctTypes.h"
#include "KwangCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UKwangCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public :
	TArray<FKwangAbilitySet> GetCombatAbilities() const {	return CombatAbilities;	}

	virtual void OnHitTargetActor(AActor* HitActor) override;
protected:
	// Abilities
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputTag"))
	TArray<FKwangAbilitySet> CombatAbilities;
};
