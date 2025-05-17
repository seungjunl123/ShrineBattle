// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DynastyGameplayAbility.generated.h"

class UPawnCombatComponent;
class UDynastyAbilitySystemComponent;

UENUM(BlueprintType)
enum class EDynastyAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
};
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDynastyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	protected:
	// Begin UGameplayAbility Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	// End Begin UGameplayAbility Interface

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EDynastyAbilityActivationPolicy AbilityActivationPolicy = EDynastyAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category="Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintPure, Category="Ability")
	UDynastyAbilitySystemComponent* GetDynastyAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EDynastySuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults);
};
