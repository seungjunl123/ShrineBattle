// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/DynastyWidgetBase.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/StartUpData/DataAsset_EnemyStartUpBase.h"
#include "DynastyFunctionLibrary.h"
#include "Engine/AssetManager.h"


AEnemyCharacter::AEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Enemy Health Widget Component"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
}

UPawnCombatComponent* AEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (UDynastyWidgetBase* DynastyWidgetBase = Cast<UDynastyWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		DynastyWidgetBase->InitEnemyCreatedWidget(this);
	}
	
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartupData();
}

void AEnemyCharacter::InitEnemyStartupData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartUpBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(KwangAbilitySystemComponent);
				}

			}
		)
	);
}

UPawnUIComponent* AEnemyCharacter::GetPawnUiComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AEnemyCharacter::GetEnemyUiComponent() const
{
	return EnemyUIComponent;
}

void AEnemyCharacter::PerformAttackTrace(FName StartSocket, FName EndSocket)
{
	// 검의 위치를 가져옴
	FVector BaseLoc = GetMesh()->GetSocketLocation(StartSocket);
	FVector TipLoc = GetMesh()->GetSocketLocation(EndSocket);

	int Steps = 5;

	if (PrevBaseLoc.IsZero())
	{
		PrevBaseLoc = BaseLoc;
		PrevTipLoc = TipLoc;
		return;
	}
	for (int i = 0;i <= Steps; i++)
	{
		float Alpha = (float)i / Steps;
		FVector InterpBase = FMath::Lerp(PrevBaseLoc, BaseLoc, Alpha);
		FVector InterpTip = FMath::Lerp(PrevTipLoc, TipLoc, Alpha);

		FVector StartLoc = (InterpBase + InterpTip) / 2;
		FVector EndLoc = ((PrevBaseLoc + PrevTipLoc) / 2);
		FRotator Rotation = FRotationMatrix::MakeFromZ(InterpTip - InterpBase).Rotator();

		float CapsuleRadius = 10.0f; // 칼의 두께
		float CapsuleHalfHeight = FVector::Dist(BaseLoc, TipLoc) / 2; // 칼의 길이

		// 디버그 캡슐 표시
		 //DrawDebugCapsule(GetWorld(), StartLoc, CapsuleHalfHeight, CapsuleRadius, Rotation.Quaternion(), FColor::Orange, false, 3.f);

		// 충돌 감지
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // 자기 자신 무시

		TArray<FHitResult> HitResults;
		GetWorld()->SweepMultiByChannel(HitResults, StartLoc, StartLoc, Rotation.Quaternion(), ECC_GameTraceChannel1, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), Params);

		for (const FHitResult& Hit : HitResults)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HittedActors.Contains(HitActor))
					continue;
				HittedActors.Add(HitActor);

				if (UDynastyFunctionLibrary::IsTargetPawnHostile(this, Cast<APawn>(HitActor)))
				{
					//UE_LOG(LogTemp, Display, TEXT("HitActor %s"),*HitActor->GetActorNameOrLabel());
					OnAttackHitTarget.ExecuteIfBound(HitActor);
				}
			}
		}
	}
	// 현재 프레임을 이전 프레임으로 저장
	PrevBaseLoc = BaseLoc;
	PrevTipLoc = TipLoc;
}

void AEnemyCharacter::ResetAttack()
{
	PrevBaseLoc = FVector::ZeroVector;
	PrevTipLoc = FVector::ZeroVector;
	HittedActors.Reset();
}