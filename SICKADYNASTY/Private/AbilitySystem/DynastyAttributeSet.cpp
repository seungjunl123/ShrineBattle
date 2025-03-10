// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DynastyAttributeSet.h"
#include "KwangGameplayTags.h"
#include "DynastyFunctionLibrary.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "GameplayEffectExtension.h"

UDynastyAttributeSet::UDynastyAttributeSet()
{
	InitMaxHealth(1.f);
	InitMaxMana(1.f);
	InitCurrentHealth(1.f);
	InitCurrentMana(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UDynastyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUiComponent();

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		const float NewCurrentMana = FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana());

		SetCurrentMana(NewCurrentMana);

		if (UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUiComponent())
		{
			PlayerUIComponent->OnCurrentManaChanged.Broadcast(GetCurrentMana() / GetMaxMana());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (GetCurrentHealth() == 0.f)
		{
			
			UDynastyFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), KwangGameplayTags::Shared_Status_Death);
		}

	}
}
