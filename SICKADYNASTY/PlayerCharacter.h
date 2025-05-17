// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "PlayerCharacter.generated.h"

class UPlayerUIComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class UComboActionData;
class UDataAsset_InputConfig;
class UKwangCombatComponent;
struct FInputActionValue;

UCLASS(config = Game)
class SICKADYNASTY_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterData, meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UKwangCombatComponent* KwangCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UPlayerUIComponent* PlayerUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> CharacterSkillIconTexture;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// UPawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	virtual void PerformAttackTrace(FName StartSocket, FName EndSocket) override; 
	virtual void ResetAttack() override;

	// UPawnUIInterface
	virtual UPawnUIComponent* GetPawnUiComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUiComponent() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Input_SwitchargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchargetCompleted(const FInputActionValue& InputActionValue);

	void Roll(const FInputActionValue& Value);
	void Input_PickUpStonesStarted(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);

protected:
	virtual void PossessedBy(AController* NewController) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE UKwangCombatComponent* GetKwangCombatComponent() const {return KwangCombatComponent; }

// 공격 관련!!!
private:
	// 공격 판정을 위한 무기 위치!
	FVector PrevBaseLoc;
	FVector PrevTipLoc;
	TSet<AActor*> HittedActors;

	float LastInputTime = 0.0f;	
// 스위칭 관련
private:
	FVector2D SwitchDirection = FVector2D::ZeroVector;


};
