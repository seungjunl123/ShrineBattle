// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DynastySurvivalGameMode.h"
#include "Character/EnemyCharacter.h"
#include "DynastyFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Engine/TargetPoint.h"
#include "Engine/AssetManager.h"

void ADynastySurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(EnemyWaveSpawnerDataTable);

	SetCurrentSurvialGameModeState(EDynastySurvialGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

    PreLoadNextWaveEnemies();

}

void ADynastySurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (CurrentSurvivalGameModeState == EDynastySurvialGameModeState::WaitSpawnNewWave)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
        {
            TimePassedSinceStart = 0.f;

            SetCurrentSurvialGameModeState(EDynastySurvialGameModeState::SpawningNewWave);
        }
    }

    if (CurrentSurvivalGameModeState == EDynastySurvialGameModeState::SpawningNewWave)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
        {
            CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

            TimePassedSinceStart = 0.f;

            SetCurrentSurvialGameModeState(EDynastySurvialGameModeState::InProgress);
        }
    }

    if (CurrentSurvivalGameModeState == EDynastySurvialGameModeState::WaveCompleted)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= WaveCompletedWaitTime)
        {
            TimePassedSinceStart = 0.f;

            CurrentWaveCount++;

            if (HasFinishedAllWaves())
            {
                SetCurrentSurvialGameModeState(EDynastySurvialGameModeState::AllWavesDone);
            }
            else
            {
                SetCurrentSurvialGameModeState(EDynastySurvialGameModeState::WaitSpawnNewWave);
                PreLoadNextWaveEnemies();

            }
        }
    }
}

void ADynastySurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    EDynastyGameDifficulty SavedGameDifficulty;

    if (UDynastyFunctionLibrary::TryLoadSavedGameDifficulty(SavedGameDifficulty))
    {
        CurrentGameDifficulty = SavedGameDifficulty;
    }
}

void ADynastySurvivalGameMode::SetCurrentSurvialGameModeState(EDynastySurvialGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;

	OnSurvialGameModeStateChanged.Broadcast(InState);
}

void ADynastySurvivalGameMode::PreLoadNextWaveEnemies()
{
    if (HasFinishedAllWaves())
    {
        return;
    }

    PreLoadedEnemyClassMap.Empty();

    for (const FDynastyEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateLambda(
                [SpawnerInfo, this]()
                {
                    if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
                    {
                        PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
                    }
                }
            )
        );
    }
}

FDynastyEnemyWaveSpawnerTableRow* ADynastySurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
    const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

    FDynastyEnemyWaveSpawnerTableRow* CurrentRow = EnemyWaveSpawnerDataTable->FindRow<FDynastyEnemyWaveSpawnerTableRow>(RowName, FString());
    
    check(CurrentRow);
    
    return CurrentRow;
}

int32 ADynastySurvivalGameMode::TrySpawnWaveEnemies()
{
    if (TargetPointsArray.IsEmpty())
    {
        UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
    }

    check(!TargetPointsArray.IsEmpty());

    uint32 EnemiesSpawnedThisTime = 0;

    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (const FDynastyEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

        const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

        UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

        for (int32 i = 0; i < NumToSpawn; i++)
        {
            const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
            const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
            const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

            FVector RandomLocation;
            UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

            RandomLocation += FVector(0.f, 0.f, 150.f);

            AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);

            if (SpawnedEnemy)
            {
                SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestyoed);
                 
                EnemiesSpawnedThisTime++;
                TotalSpawnedEnemiesThisWaveCounter++;
            }

            if (!ShouldKeepSpawnEnemies())
            {
                return EnemiesSpawnedThisTime;
            }
        }
    }

    return EnemiesSpawnedThisTime;
}

bool ADynastySurvivalGameMode::ShouldKeepSpawnEnemies() const
{
    return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void ADynastySurvivalGameMode::OnEnemyDestyoed(AActor* DestroyActor)
{
    CurrentSpawnedEnemiesCounter--;


    if (ShouldKeepSpawnEnemies())
    {
        CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
    }
    else if (CurrentSpawnedEnemiesCounter == 0)
    {
        TotalSpawnedEnemiesThisWaveCounter = 0;
        CurrentSpawnedEnemiesCounter = 0;

        SetCurrentSurvialGameModeState(EDynastySurvialGameModeState::WaveCompleted);
    }
}

bool ADynastySurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void ADynastySurvivalGameMode::RegisterSpawnedEnemies(const TArray<AEnemyCharacter*>& InEnemiesToRegister)
{
    for (AEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
    {
        if (SpawnedEnemy)
        {
            CurrentSpawnedEnemiesCounter++;

            SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestyoed);
        }
    }
}
