// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Abilities/DynastyGameplayAbility.h"
#include "KwangGameplayAbility.generated.h"

class APlayerCharacter;
class ADynastyPlayerController;
class UKwangCombatComponent;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UKwangGameplayAbility : public UDynastyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Ability")
	APlayerCharacter* GetPlayerCharacterFromActorInfo();
	UFUNCTION(BlueprintPure, Category="Ability")
	ADynastyPlayerController* GetPlayerControllerFromActorInfo();
	UFUNCTION(BlueprintPure, Category="Ability")
	UKwangCombatComponent* GetKwangCombatComponentFromActorInfo();
	UFUNCTION(BlueprintPure, Category="Ability")
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentCombocount);

private:
	TWeakObjectPtr<APlayerCharacter> ChachedPlayerCharacter;
	TWeakObjectPtr<ADynastyPlayerController> ChachedDynastyPlayerController;
};
