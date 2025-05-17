// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DynastyTypes/DynastyEnumTypes.h"
#include "DynastyFunctionLibrary.generated.h"

class UDynastyAbilitySystemComponent;
class UPawnCombatComponent;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UDynastyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UDynastyAbilitySystemComponent* NativeGetDynastyASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs="OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EDynastyConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponent(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponent(AActor* InActor, EDynastyValidType& OutValidType);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,float& OutRemainingTime, EDynastyCountDownActionInput CountDownInput, EDynastyCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static class UDynastyGameInstance* GetDynastyGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EDynastyInputMode InInputMode);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void SaveCurrentGameDifficulty(EDynastyGameDifficulty InDifficultyToSave);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool TryLoadSavedGameDifficulty(EDynastyGameDifficulty& OutSavedDifficulty);
};
