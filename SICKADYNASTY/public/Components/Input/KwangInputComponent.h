// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "KwangInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UKwangInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject,typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InputConfig, const FGameplayTag& Tag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject,typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InputConfig,UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

template <class UserObject, typename CallbackFunc>
inline void UKwangInputComponent::BindNativeInputAction(const UDataAsset_InputConfig *InputConfig, const FGameplayTag &InputTag, ETriggerEvent TriggerEvent, UserObject *ContextObject, CallbackFunc Func)
{
	checkf(InputConfig, TEXT("InputConfig is null")) // InputConfig이 false면 Text를 디버깅 출력

	if(UInputAction* FoundAction =InputConfig->FindNativeInputActionByTag(InputTag))
	{
		BindAction(FoundAction,TriggerEvent,ContextObject, Func);
	}
}

template <class UserObject, typename CallbackFunc>
inline void UKwangInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig *InputConfig, UserObject *ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InputConfig,TEXT("Input Config data asset is null, can not proceed with binding"));

	for(const FKwangInputActionConfig& AbilityInputActionConfig : InputConfig->AbilityInputActions)
	{
		if(!AbilityInputActionConfig.IsValid()) continue;

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc,AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc,AbilityInputActionConfig.InputTag);
	}
}
