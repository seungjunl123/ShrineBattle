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
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag); // 태그를 가진 이벤트 콜백을 가져온다.

	Handle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived); // 나중에 Remove를 위해 변수로 저장
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
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad( // 비동기 스트리밍(로딩) 관리자 
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
			// ↑ 비동기 로딩 완료 후 델리게이트
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

	FActorSpawnParameters SpawnParam; // 액터 생성 시 설정할 수 있는 구조체
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// 충돌하지 않는 위치를 찾아서 스폰, 못찾으면 AlwaysSpawn으로 적용
	for (int32 i = 0;i < CachedNumToSpawn;i++)
	{
		FVector RandomLocation;
		
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);
		// ↑ NavMesh에서 도달할 수 있는 위치 반환 -> RandomLocation에 저장됨
		RandomLocation += FVector(0.f, 0.f, 100.f); // 스폰된 액터 바닥에 안끼이게

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
