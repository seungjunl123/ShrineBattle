// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/KwangCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "KwangGameplayTags.h"

void UKwangCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		KwangGameplayTags::Shared_Event_MeleeHit,
		Data
	);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		KwangGameplayTags::Player_Event_HitShake,
		FGameplayEventData()
	);
}
