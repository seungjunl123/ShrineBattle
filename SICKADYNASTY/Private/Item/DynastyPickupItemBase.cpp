// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DynastyPickupItemBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ADynastyPickupItemBase::ADynastyPickupItemBase()
{
    PrimaryActorTick.bCanEverTick = false;

    PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
    SetRootComponent(PickUpCollisionSphere);
    PickUpCollisionSphere->InitSphereRadius(50.f);
    PickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpCollisionSphereBeginOverlap);

}

void ADynastyPickupItemBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


