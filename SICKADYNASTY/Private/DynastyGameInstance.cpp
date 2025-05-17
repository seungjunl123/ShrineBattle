// Fill out your copyright notice in the Description page of Project Settings.


#include "DynastyGameInstance.h"
#include "MoviePlayer.h"

void UDynastyGameInstance::Init()
{
    Super::Init();

    // 로딩 화면을 위한 델리게이트
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap); 
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}
void UDynastyGameInstance::OnPreLoadMap(const FString& MapName)
{
    FLoadingScreenAttributes LoadingScreenAttributes;
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f; // 최소 로딩 시간
    LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget(); // 기본 로딩 창( ... 이 애니메이션 동작하는 걸로 기본 구성)

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes); // 게임 스레드가 아닌 다른 스레드에서 로딩 화면이 동작
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
