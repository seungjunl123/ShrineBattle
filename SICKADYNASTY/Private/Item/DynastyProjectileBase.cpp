// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DynastyProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "DynastyFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "KwangGameplayTags.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ADynastyProjectileBase::ADynastyProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

    ProjectileCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
    SetRootComponent(ProjectileCollisionComponent);
    ProjectileCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovementComponent->InitialSpeed = 700.f;
    ProjectileMovementComponent->MaxSpeed = 900.f;
    ProjectileMovementComponent->Velocity = FVector(1.f, 0.f, 0.f);
    ProjectileMovementComponent->ProjectileGravityScale = 0.f; // �������� �ʰ� �÷��̾� ����

    InitialLifeSpan = 4.f;
    
    // ��Ʈ �� ��������Ʈ (OnComponentHit���� FComponentHitSignature����)
    ProjectileCollisionComponent->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
    ProjectileCollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

}

// Called when the game starts or when spawned
void ADynastyProjectileBase::BeginPlay()
{
	Super::BeginPlay();

    if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap) // ������ Ÿ�Խ� ������ �ø��� ä�� ����
    {
        ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    }
}


void ADynastyProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    BP_OnSpawnProjectileHitEffect(Hit.ImpactPoint); // ���� ������ �Ҹ� ����

    APawn* HitPawn = Cast<APawn>(OtherActor);

    if (!HitPawn || !UDynastyFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
    {
        Destroy();
        return;
    }

    bool bIsValidBlock = false;
    const bool bIsPlayerBlocking = UDynastyFunctionLibrary::NativeDoesActorHaveTag(HitPawn, KwangGameplayTags::Player_State_Blocking);

    if (bIsPlayerBlocking)
    {
        bIsValidBlock = UDynastyFunctionLibrary::IsValidBlock(GetInstigator(), HitPawn);
    }

    FGameplayEventData Data;
    Data.Instigator = this;
    Data.Target = HitPawn;

    if (bIsValidBlock)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            HitPawn,
            KwangGameplayTags::Player_Event_SuccessfulBlock,
            Data
        );
    }
    else
    {
        HandleApplyProjectileDamage(HitPawn, Data);
    }

    Destroy();
}

void ADynastyProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OverlappedActors.Contains(OtherActor))
    {
        return;
    }

    OverlappedActors.AddUnique(OtherActor);

    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        FGameplayEventData Data;
        Data.Instigator = GetInstigator();
        Data.Target = HitPawn;

        if (UDynastyFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
        {
            HandleApplyProjectileDamage(HitPawn, Data);
        }
    }
}

void ADynastyProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
    const bool bWasApplied = UDynastyFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);

    if (bWasApplied)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            InHitPawn,
            KwangGameplayTags::Shared_Event_HitReact,
            InPayload
        );
    }
}


