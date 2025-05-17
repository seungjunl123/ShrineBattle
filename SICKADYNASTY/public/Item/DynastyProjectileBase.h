// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DynastyProjectileBase.generated.h"

class UParticleSystem;
class UBoxComponent;
class UProjectileMovementComponent;

UENUM()
enum class EProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

UCLASS()
class SICKADYNASTY_API ADynastyProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynastyProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle; // 투사체의 경우 SpecHandle를 변수로 관리


	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitEffect(const FVector& HitLocation);

public:
	void SetProjectileDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InSpec)
	{
		ProjectileDamageEffectSpecHandle = InSpec;
	}
private:
	TArray<AActor*> OverlappedActors;

};
