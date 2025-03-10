// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "DynastyFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ComboActionData.h"
#include "Components/Input/KwangInputComponent.h"
#include "KwangGameplayTags.h"
#include "DataAsset/StartUpData/DataAsset_StartUpBase.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "Components/Combat/KwangCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"

#include "../SICKADYNASTY/SICKADYNASTY.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	KwangCombatComponent = CreateDefaultSubobject<UKwangCombatComponent>(TEXT("KwangCombatComponent"));
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

UPawnCombatComponent* APlayerCharacter::GetPawnCombatComponent() const
{
	return KwangCombatComponent;
}

UPawnUIComponent* APlayerCharacter::GetPawnUiComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* APlayerCharacter::GetPlayerUiComponent()
{
	return PlayerUIComponent;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	KwangAbilitySystemComponent->GrantPlayerWeaponAbility(KwangCombatComponent->GetCombatAbilities(), 1);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!CharacterStartUpData.IsNull()) // 소프트 참조의 경우 IsValid가 아닌 IsNull로 체크 -> IsValid는 에셋이 로드된 경우임
	{
		if(UDataAsset_StartUpBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(KwangAbilitySystemComponent);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset,TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	UKwangInputComponent* KwangInputComponent = CastChecked<UKwangInputComponent>(PlayerInputComponent);

	KwangInputComponent->BindNativeInputAction(InputConfigDataAsset,KwangGameplayTags::InputTag_Move,ETriggerEvent::Triggered,this,&ThisClass::Move);
	KwangInputComponent->BindNativeInputAction(InputConfigDataAsset,KwangGameplayTags::InputTag_Look,ETriggerEvent::Triggered,this,&ThisClass::Look);
	KwangInputComponent->BindNativeInputAction(InputConfigDataAsset, KwangGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchargetTriggered);
	KwangInputComponent->BindNativeInputAction(InputConfigDataAsset, KwangGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchargetCompleted);

	KwangInputComponent->BindAbilityInputAction(InputConfigDataAsset,this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{

	
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Input_SwitchargetTriggered(const FInputActionValue& InputActionValue)
{
	// 마우스가 이동한 방향
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void APlayerCharacter::Input_SwitchargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	// 왼쪽 오른쪽 적을 스위칭 하도록 이벤트 보내자
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ? KwangGameplayTags::Player_Event_SwitchTarget_Right : KwangGameplayTags::Player_Event_SwitchTarget_Left,
		Data

	);
}

void APlayerCharacter::Roll(const FInputActionValue &Value)
{
	
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	// input에 대한 바인딩은 캐릭터에서 담당하고고
	// 어빌리티 활성화는 ASC에서 담당함!
	KwangAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	KwangAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}

//////////////////////////////////////////////////////////////////////////
// Combat

void APlayerCharacter::PerformAttackTrace(FName StartSocket, FName EndSocket)
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
	for(int i=0;i<= Steps; i++) // 보간을 통한 충돌 감지 세부화
	{
		float Alpha = (float)i / Steps;
        FVector InterpBase = FMath::Lerp(PrevBaseLoc, BaseLoc, Alpha);
        FVector InterpTip = FMath::Lerp(PrevTipLoc, TipLoc, Alpha);
 
    	FVector StartLoc = (InterpBase + InterpTip) / 2;
		FVector EndLoc = ((PrevBaseLoc + PrevTipLoc) / 2);
    	FRotator Rotation = FRotationMatrix::MakeFromZ(InterpTip-InterpBase).Rotator();
    	
		float CapsuleRadius = 10.0f; 
    	float CapsuleHalfHeight = FVector::Dist(BaseLoc, TipLoc) / 2; // 칼의 길이

    	// 디버그 캡슐 표시
		// DrawDebugCapsule(GetWorld(), StartLoc, CapsuleHalfHeight, CapsuleRadius, Rotation.Quaternion(), FColor::Red, false, 3.f);
    	
		// 충돌 감지
    	FCollisionQueryParams Params;
    	Params.AddIgnoredActor(this); // 자기 자신 무시

    	TArray<FHitResult> HitResults;
    	GetWorld()->SweepMultiByChannel(
			HitResults, StartLoc, StartLoc, Rotation.Quaternion(), 
			ECC_GameTraceChannel1, 
			FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), 
			Params
		);

    	for (const FHitResult& Hit : HitResults)
    	{
        	if (AActor* HitActor = Hit.GetActor())
       	 	{
				if(HittedActors.Contains(HitActor))
					continue;
				HittedActors.Add(HitActor);
				// 적 판단 후 바인딩된 델리케이트 실행
				if (UDynastyFunctionLibrary::IsTargetPawnHostile(this, Cast<APawn>(HitActor)))
				{
					OnAttackHitTarget.ExecuteIfBound(HitActor);
				}
        	}
    	}
	}
    // 현재 프레임을 이전 프레임으로 저장
    PrevBaseLoc = BaseLoc;
    PrevTipLoc = TipLoc;
}

void APlayerCharacter::ResetAttack()
{
	PrevBaseLoc = FVector::ZeroVector;
    PrevTipLoc = FVector::ZeroVector;
	HittedActors.Reset();
}

