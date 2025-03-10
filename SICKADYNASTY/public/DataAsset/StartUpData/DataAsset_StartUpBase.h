// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpBase.generated.h"


class UDynastyGameplayAbility;
class UDynastyAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDataAsset_StartUpBase : public UDataAsset
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UDynastyAbilitySystemComponent* InASCToGive, int32 AbilityLevel = 1);

protected:
	// 주어진 직후에 활성화 해야하는 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UDynastyGameplayAbility>> ActivateOnGivenAbilities;
	// 공격, 타격, 사망 등 특정 조건에서 발동하는 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UDynastyGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UDynastyGameplayAbility>>& InAbilityToGive, UDynastyAbilitySystemComponent* InASCToGive, int32 AbilityLevel = 1);
};
