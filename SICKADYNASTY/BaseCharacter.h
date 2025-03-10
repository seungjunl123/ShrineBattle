// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "BaseCharacter.generated.h"

class UDynastyAbilitySystemComponent;
class UDynastyAttributeSet;
class UDataAsset_StartUpBase;
class UMotionWarpingComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

UCLASS()
class SICKADYNASTY_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface /* GAS가 해당 액터를 인식하기 위해 상속이 꼭 필요*/,  public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// UPawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

	// UPawnUIInterface
	virtual UPawnUIComponent* GetPawnUiComponent() const override;

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta=(AllowPrivateAccess="true"))
	UDynastyAbilitySystemComponent* KwangAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta=(AllowPrivateAccess="true"))
	UDynastyAttributeSet* BasicAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChracterData")
	TSoftObjectPtr<UDataAsset_StartUpBase> CharacterStartUpData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;
public:
	FORCEINLINE UDynastyAbilitySystemComponent* GetKwangAbilitySystemComponent() const {return KwangAbilitySystemComponent;}
	
	FORCEINLINE UDynastyAttributeSet* GetAttributeSet() const {return BasicAttributeSet;}

	FOnTargetInteractedDelegate OnAttackHitTarget; 
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
