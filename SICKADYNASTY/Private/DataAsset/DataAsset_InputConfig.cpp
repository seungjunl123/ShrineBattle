// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_InputConfig.h"

UInputAction *UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag &InputTag) const
{
    for(const FKwangInputActionConfig& InputActionConfig : NativeInputActions)
    {
        if(InputActionConfig.InputTag == InputTag && InputActionConfig.InputAction)
        {
            return InputActionConfig.InputAction;
        }
    }

    return nullptr;
}