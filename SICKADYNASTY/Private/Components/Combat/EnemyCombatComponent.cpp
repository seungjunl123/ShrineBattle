// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DynastyFunctionLibrary.h"
#include "KwangGameplayTags.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UDynastyFunctionLibrary::NativeDoesActorHaveTag(HitActor,KwangGameplayTags::Player_State_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UDynastyFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}
	
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			KwangGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
	}
	else
	{

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			KwangGameplayTags::Shared_Event_MeleeHit,
			Data
		);
	}
}
