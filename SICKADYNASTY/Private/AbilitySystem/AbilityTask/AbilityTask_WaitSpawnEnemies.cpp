// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Character/EnemyCharacter.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask< UAbilityTask_WaitSpawnEnemies>(OwningAbility);
	
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag); // �±׸� ���� �̺�Ʈ �ݹ��� �����´�.

	Handle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived); // ���߿� Remove�� ���� ������ ����
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.Remove(Handle);

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad( // �񵿱� ��Ʈ����(�ε�) ������ 
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
			// �� �񵿱� �ε� �Ϸ� �� ��������Ʈ
		);
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates()) 
		{
			DidNotSpawn.Broadcast(TArray<AEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AEnemyCharacter*>());
		}

		EndTask();
		return;

	}

	TArray<AEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParam; // ���� ���� �� ������ �� �ִ� ����ü
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// �浹���� �ʴ� ��ġ�� ã�Ƽ� ����, ��ã���� AlwaysSpawn���� ����
	for (int32 i = 0;i < CachedNumToSpawn;i++)
	{
		FVector RandomLocation;
		
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);
		// �� NavMesh���� ������ �� �ִ� ��ġ ��ȯ -> RandomLocation�� �����
		RandomLocation += FVector(0.f, 0.f, 100.f); // ������ ���� �ٴڿ� �ȳ��̰�

		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		AEnemyCharacter* SpawnedEnemy = World->SpawnActor<AEnemyCharacter>(LoadedClass,RandomLocation, SpawnFacingRotation, SpawnParam);
		
		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AEnemyCharacter*>());
		}
	}

	EndTask();
}
