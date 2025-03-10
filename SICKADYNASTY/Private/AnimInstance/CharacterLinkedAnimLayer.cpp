// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterLinkedAnimLayer.h"
#include "AnimInstance/CharacterKwangAnimInstance.h"

UCharacterKwangAnimInstance *UCharacterLinkedAnimLayer::GetPlayerAnimInstance() const
{
    return Cast<UCharacterKwangAnimInstance>(GetOwningComponent()->GetAnimInstance());
}