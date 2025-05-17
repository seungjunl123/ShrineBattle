// Fill out your copyright notice in the Description page of Project Settings.


#include "DynastyGameInstance.h"
#include "MoviePlayer.h"

void UDynastyGameInstance::Init()
{
    Super::Init();

    // �ε� ȭ���� ���� ��������Ʈ
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap); 
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}
void UDynastyGameInstance::OnPreLoadMap(const FString& MapName)
{
    FLoadingScreenAttributes LoadingScreenAttributes;
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f; // �ּ� �ε� �ð�
    LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget(); // �⺻ �ε� â( ... �� �ִϸ��̼� �����ϴ� �ɷ� �⺻ ����)

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes); // ���� �����尡 �ƴ� �ٸ� �����忡�� �ε� ȭ���� ����
}

void UDynastyGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
    GetMoviePlayer()->StopMovie();
}


TSoftObjectPtr<UWorld> UDynastyGameInstance::GetGameLevelByTag(FGameplayTag InTag)
{
    for (const FDynastyGameLevelSet& GameLevelSet : GameLevelSets)
    {
        if (!GameLevelSet.IsValid()) continue;

        if (GameLevelSet.LevelTag == InTag)
        {
            return GameLevelSet.Level;
        }
    }

    return TSoftObjectPtr<UWorld>();
}
