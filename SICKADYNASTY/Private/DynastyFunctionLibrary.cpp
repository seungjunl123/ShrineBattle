// Fill out your copyright notice in the Description page of Project Settings.


#include "DynastyFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "KwangGameplayTags.h"
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
        ASC->AddLooseGameplayTag(TagToAdd); // �� ���� �Ѱ��� �±׸� ���� �� ���
    }
}

void UDynastyFunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UDynastyAbilitySystemComponent* ASC = NativeGetDynastyASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove); // �� ���� �Ѱ��� �±׸� ���� �� ���
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
    check(QueryPawn && TargetPawn);

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

    // ���� ���
    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    // ���� ��� -> �´� ����� �����̴ϱ� VictimForward�� ù��° ���ڴ�.
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
    
    return KwangGameplayTags::Shared_Status_HitReact_Front;
}

bool UDynastyFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
    check(InAttacker && InDefender);

    // ���̶� �÷��̾� ������ 80�� ���� �̳��� ��� �۵�
    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

    UE_LOG(LogTemp, Display, TEXT("DotResult %f"), DotResult);
    return DotResult < -0.2f;
}
