// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"


class AEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AEnemyCharacter*>&, SpawnEnemies);
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (DisplayName = "Wait Gameplay Event And Spawn Enemy", HidePin = "OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true", NumToSpawn="1", RandomSpawnRadius="200"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<AEnemyCharacter> SoftEnemyClassToSpawn,
		int32 NumToSpawn,
		const FVector& SpawnOrigin,
		float RandomSpawnRadius
	);
	
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;

	// UGameplayTask interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;

	FDelegateHandle Handle;

	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnEnemyClassLoaded();
};
