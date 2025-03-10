// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KwangGameplayAbility_TargetLock.h"
#include "DynastyFunctionLibrary.h"
#include "KwangGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/DynastyWidgetBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "SICKADYNASTY/PlayerCharacter.h"
#include "DynastyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Abilities/KwangGameplayAbility.h"

void UKwangGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	InitTargetLockMappingContext();
	TryLockonTarget();

	Super::ActivateAbility( Handle,  ActorInfo,  ActivationInfo,  TriggerEventData);
}

void UKwangGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanUp();
	ResetTargetLockMappongContext();
	Super::EndAbility( Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UKwangGameplayAbility_TargetLock::TryLockonTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();
	}
	else
	{
		CancelTargetLockAbility();
	}

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();
		SetTargetLockWidgetPosition();
	}
}

void UKwangGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	TArray<FHitResult> BoxTraceHits;


	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(), // 월드 컨텍스트 오브젝트
		GetPlayerCharacterFromActorInfo()->GetActorLocation(), // 시작점
		GetPlayerCharacterFromActorInfo()->GetActorLocation() + GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance, // 끝점. 트레이싱 거리는 5000까지 했다
		TraceBoxSize / 2.f, // 박스 사이즈의 절반
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(), // 회전
		BoxTraceChannel, // 뭐 감지할 건지 -> Pawn으로 설정
		false, // 복합 컬리전을 탐지하는지 -> Nope
		TArray<AActor*>(), // 무시할 액터 목록 .. 없음
		bShowPersistanceDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None, // 디버깅
		BoxTraceHits, // 산출되는 FHItResult
		true // 
	);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			UPrimitiveComponent* HitComponent = TraceHit.GetComponent(); // 충돌한 컴포넌트 확인
			

			if (HitActor != GetPlayerCharacterFromActorInfo())
			{
				UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s, Component: %s"),
					*HitActor->GetActorNameOrLabel(),
					*HitComponent->GetName()); // 어떤 콜라이더가 감지되는지 출력

				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

void UKwangGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (DrawnTargetLockWidget) return;

	checkf(TargetLockWidgetClass, TEXT("Forget to assign a valid widget class in blueprint"));

	DrawnTargetLockWidget = CreateWidget<UDynastyWidgetBase>(GetPlayerControllerFromActorInfo(), TargetLockWidgetClass);

	check(DrawnTargetLockWidget);

	DrawnTargetLockWidget->AddToViewport();
}

void UKwangGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}
	FVector2D ScreenPosition;

	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);
	// 위젯 사이즈 수정
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	if (UCapsuleComponent* LockedActorCapsuleComponent = CurrentLockedActor->FindComponentByClass<UCapsuleComponent>())
	{
		ScreenPosition.Y -= LockedActorCapsuleComponent->GetUnscaledCapsuleHalfHeight() * 2.0f;
	}
	ScreenPosition.X -= (TargetLockWidgetSize.X / 2.f);

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);

}

void UKwangGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);
	Subsystem->AddMappingContext(TargetLockMappingContext, 3); // 우선도 높이기 
}


void UKwangGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UKwangGameplayAbility_TargetLock::ResetTargetLockMappongContext()
{
	if (!GetPlayerControllerFromActorInfo())
	{
		return; // 락온 시 게임을 종료하면 어빌리티가 종료되면서 Reset 함수가 실행되는데, 그 때 히어로 컨트롤러가 없어서 충돌 발생
		// 이를 해결하기 위해서 컨트롤러 nullptr 체크
	}
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockMappingContext);

}


void UKwangGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}
	DrawnTargetLockWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;
}

AActor* UKwangGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);

}

void UKwangGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
		
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossResult.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void UKwangGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor ||
		UDynastyFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, KwangGameplayTags::Shared_Status_Death) ||
		UDynastyFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), KwangGameplayTags::Shared_Status_Death)
		)
	{
		CancelTargetLockAbility();

		return;
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation =
		!UDynastyFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), KwangGameplayTags::Player_State_Roll)
		&&
		!UDynastyFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), KwangGameplayTags::Player_State_Blocking);
	
	if (bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetPlayerCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		LookAtRot -= FRotator(20.f, 0.f, 0.f);

		// Character의 카메라암이 컨트롤러 회전의 영향을 받으므로, 컨트롤러를 회전 시킨다.
		const FRotator CurrentControlRot = GetPlayerControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetPlayerControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UKwangGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetActor = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == KwangGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetActor = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetActor = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetActor)
	{
		CurrentLockedActor = NewTargetActor;
	}
}
