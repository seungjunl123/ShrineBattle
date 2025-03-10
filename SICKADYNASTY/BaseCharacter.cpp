// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "AbilitySystem/DynastyAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	// HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthSet"));

	KwangAbilitySystemComponent = CreateDefaultSubobject<UDynastyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	BasicAttributeSet = CreateDefaultSubobject<UDynastyAttributeSet>(TEXT("BasicAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

UAbilitySystemComponent *ABaseCharacter::GetAbilitySystemComponent() const
{
    return KwangAbilitySystemComponent;
}


UPawnCombatComponent* ABaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* ABaseCharacter::GetPawnUiComponent() const
{
	return nullptr;
}


void ABaseCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
	if(KwangAbilitySystemComponent)
	{
		KwangAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(),TEXT("Forgot to assign startup Data to %s"), *GetName()); // 에러 발생 시 중단지점 트리거
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
