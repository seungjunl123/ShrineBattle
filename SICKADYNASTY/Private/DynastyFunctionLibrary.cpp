// Fill out your copyright notice in the Description page of Project Settings.


#include "DynastyFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "DynastyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/DynastySaveGame.h"
#include "KwangGameplayTags.h"
#include "Item/DynastyCountDownAction.h"
#include "AbilitySystemBlueprintLibrary.h"

UDynastyAbilitySystemComponent* UDynastyFunctionLibrary::NativeGetDynastyASCFromActor(AActor* InActor)
{
    check(InActor);
    return CastChecked<UDynastyAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));


}

void UDynastyFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UDynastyAbilitySystemComponent* ASC = NativeGetDynastyASCFromActor(InActor);

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd); // 한 번에 한개의 태그만 붙일 때 사용
    }
}

void UDynastyFunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UDynastyAbilitySystemComponent* ASC = NativeGetDynastyASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove); // 한 번에 한개의 태그만 붙일 때 사용
    }
}

bool UDynastyFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UDynastyAbilitySystemComponent* ASC = NativeGetDynastyASCFromActor(InActor);
    
    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UDynastyFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EDynastyConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EDynastyConfirmType::Yes : EDynastyConfirmType::No;
}

UPawnCombatComponent* UDynastyFunctionLibrary::NativeGetPawnCombatComponent(AActor* InActor)
{
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }
    return nullptr;
}

UPawnCombatComponent* UDynastyFunctionLibrary::BP_GetPawnCombatComponent(AActor* InActor, EDynastyValidType& OutValidType)
{
    UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponent(InActor);

    OutValidType = PawnCombatComponent ? EDynastyValidType::Valid : EDynastyValidType::Invalid;

    return PawnCombatComponent;
}

bool UDynastyFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    if (!QueryPawn || !TargetPawn) return false;

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

FGameplayTag UDynastyFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    // 내적 계산
    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    // 외적 계산 -> 맞는 사람이 기준이니까 VictimForward가 첫번째 인자다.
    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);
    if (CrossResult.Z < 0.f)
    {
        OutAngleDifference *= -1.f;
    }

    if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
    {
        return KwangGameplayTags::Shared_Status_HitReact_Front;
    }
    else if (OutAngleDifference <= -45.f && OutAngleDifference >= -135.f)
    {
        return KwangGameplayTags::Shared_Status_HitReact_Left;
    }
    else if (OutAngleDifference <= -135.f || OutAngleDifference >= 135.f)
    {
        return KwangGameplayTags::Shared_Status_HitReact_Back;
    }
    else
    {
        return KwangGameplayTags::Shared_Status_HitReact_Right;
    }
}

bool UDynastyFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
    check(InAttacker && InDefender);

    // 적이랑 플레이어 각도가 80도 정도 이내면 방어 작동
    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

    return DotResult < -0.2f;
}

bool UDynastyFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle InSpecHandle)
{
    UDynastyAbilitySystemComponent* SourceASC = NativeGetDynastyASCFromActor(InInstigator);
    UDynastyAbilitySystemComponent* TargetASC = NativeGetDynastyASCFromActor(InTargetActor);

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UDynastyFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EDynastyCountDownActionInput CountDownInput, EDynastyCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
    UWorld* World = nullptr;

    if (GEngine)
    {
        World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    }

    if (!World)
    {
        return;
    }

    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

    FDynastyCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FDynastyCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

    if (CountDownInput == EDynastyCountDownActionInput::Start)
    {
        if (!FoundAction)
        {
            LatentActionManager.AddNewAction(
                LatentInfo.CallbackTarget,
                LatentInfo.UUID,
                new FDynastyCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
            );
        }
    }

    if (CountDownInput == EDynastyCountDownActionInput::Cancel)
    {
        if (FoundAction)
        {
            FoundAction->CancelAction();
        }
    }
}

UDynastyGameInstance* UDynastyFunctionLibrary::GetDynastyGameInstance(const UObject* WorldContextObject)
{
    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            return World->GetGameInstance<UDynastyGameInstance>();
        }
    }

    return nullptr;
}

void UDynastyFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EDynastyInputMode InInputMode)
{
    APlayerController* PlayerController = nullptr;

    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            PlayerController = World->GetFirstPlayerController();
        }
    }

    if (!PlayerController)
    {
        return;
    }

    FInputModeGameOnly GameOnlyMode;
    FInputModeUIOnly UIOnlyMode;

    switch (InInputMode)
    {
    case EDynastyInputMode::GameOnly:

        PlayerController->SetInputMode(GameOnlyMode);
        PlayerController->bShowMouseCursor = false;

        break;

    case EDynastyInputMode::UIOnly:

        PlayerController->SetInputMode(UIOnlyMode);
        PlayerController->bShowMouseCursor = true;

        break;

    default:
        break;
    }
}

void UDynastyFunctionLibrary::SaveCurrentGameDifficulty(EDynastyGameDifficulty InDifficultyToSave)
{
    USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UDynastySaveGame::StaticClass());

    if (UDynastySaveGame* DynastySaveGameObject = Cast<UDynastySaveGame>(SaveGameObject))
    {
        DynastySaveGameObject->SavedCurrentGameDifficulty = InDifficultyToSave;

        const bool bWasSaved = UGameplayStatics::SaveGameToSlot(DynastySaveGameObject, KwangGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

        if (bWasSaved)
        {
            UE_LOG(LogTemp, Display, TEXT("Save Success"));
        }
        else 
        {
            UE_LOG(LogTemp, Display, TEXT("Save Failed"));
        }
    }
}

bool UDynastyFunctionLibrary::TryLoadSavedGameDifficulty(EDynastyGameDifficulty& OutSavedDifficulty)
{
    if (UGameplayStatics::DoesSaveGameExist(KwangGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
    {
        USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(KwangGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

        if (UDynastySaveGame* DynastySaveGameObject = Cast<UDynastySaveGame>(SaveGameObject))
        {
            OutSavedDifficulty = DynastySaveGameObject->SavedCurrentGameDifficulty;

            return true;
        }
    }

    return false;
}

float UDynastyFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}


