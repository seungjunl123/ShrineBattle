// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "Character/EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DynastyFunctionLibrary.h"
#include "KwangGameplayTags.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{

	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UDynastyFunctionLibrary::NativeDoesActorHaveTag(HitActor,KwangGameplayTags::Player_State_Blocking);
	const bool bIsMyAttackUnblockable = UDynastyFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), KwangGameplayTags::Enemy_State_Unblockable);

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

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AEnemyCharacter>();

	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandBoxCollision = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandBoxCollision = OwningEnemyCharacter->GetRightHandCollisionBox();

	switch (ToggleDamageType) // Tab + 아래 화살표로 모든 케이스 가져옴
	{
	case EToggleDamageType::LeftHand:
		LeftHandBoxCollision->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		RightHandBoxCollision->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}

	if (!bShouldEnable)
	{
		OwningEnemyCharacter->ResetAttack();
	}
}
