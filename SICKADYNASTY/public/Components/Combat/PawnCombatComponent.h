// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"


UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	LeftHand,
	RightHand,
};
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()


protected:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);


public:
	UPawnCombatComponent();

	virtual void OnHitTargetActor(AActor* HitActor);


	void DelegateBindingToActor();
};
