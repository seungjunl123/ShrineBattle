// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DynastyTypes/DynastyStuctTypes.h"
#include "DynastyAbilitySystemComponent.generated.h"
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDynastyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public :
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "Ability", meta=(ApplyLevel ="1"))
	void GrantPlayerWeaponAbility(const TArray<FKwangAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel);
	void RemoveGrantedPlayerWeaponAbility(UPARAM(ref)TArray<FGameplayAbilitySpecHandle> InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTag);
	
};
