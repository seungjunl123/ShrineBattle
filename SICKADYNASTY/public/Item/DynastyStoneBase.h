// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DynastyPickupItemBase.h"
#include "DynastyStoneBase.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API ADynastyStoneBase : public ADynastyPickupItemBase
{
	GENERATED_BODY()
public:
	void Consume(class UDynastyAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);
protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> StoneGameplayEffectClass;
};
