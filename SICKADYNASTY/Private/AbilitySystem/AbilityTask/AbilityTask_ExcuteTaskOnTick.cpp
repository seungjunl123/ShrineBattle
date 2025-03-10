// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AbilityTask_ExcuteTaskOnTick.h"

UAbilityTask_ExcuteTaskOnTick::UAbilityTask_ExcuteTaskOnTick()
{
	bTickingTask = true;
}

UAbilityTask_ExcuteTaskOnTick* UAbilityTask_ExcuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UAbilityTask_ExcuteTaskOnTick* Node = NewAbilityTask<UAbilityTask_ExcuteTaskOnTick>(OwningAbility);
	return Node;
}

void UAbilityTask_ExcuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}
