// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterKwangAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance\CharacterKwangAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "SICKADYNASTY\BaseCharacter.h"

void UCharacterKwangAnimInstance::NativeInitializeAnimation()
{
    OwningCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

    if(OwningCharacter) // 블루프린트에서 가져오는 변수는 check 매크로 사용 불가
    {
        OwningMovementComponent = OwningCharacter->GetCharacterMovement();
    }
}

void UCharacterKwangAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    if(!OwningCharacter || !OwningMovementComponent)
    {
        return;
    }

    GroundSpeed = OwningCharacter->GetVelocity().Size2D();
    bHasAccelation = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
    LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());

}
