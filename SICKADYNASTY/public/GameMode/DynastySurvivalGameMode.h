// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/DynastyGameModeBase.h"
#include "DynastySurvivalGameMode.generated.h"

class AEnemyCharacter;

UENUM(BlueprintType)
enum class EDynastySurvialGameModeState : uint8
{
    WaitSpawnNewWave,
    SpawningNewWave,
    InProgress,
    WaveCompleted,
    AllWavesDone,
    PlayerDied
};

USTRUCT(BlueprintType)
struct FDynastyEnemyWaveSpawnerInfo // Table Row에 들어갈 적 클래스 정보
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TSoftClassPtr<AEnemyCharacter> SoftEnemyClassToSpawn;

    UPROPERTY(EditAnywhere)
    int32 MinPerSpawnCount = 1;

    UPROPERTY(EditAnywhere)
    int32 MaxPerSpawnCount = 3;
};

USTRUCT(BlueprintType)
struct FDynastyEnemyWaveSpawnerTableRow : public FTableRowBase // Wave 정보가 들어갈 데이터 에셋의 Row 정의
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<FDynastyEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;

    UPROPERTY(EditAnywhere)
    int32 TotalEnemyToSpawnThisWave = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate, EDynastySurvialGameModeState, CurrentState);

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API ADynastySurvivalGameMode : public ADynastyGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
    void SetCurrentSurvialGameModeState(EDynastySurvialGameModeState InState);
    void PreLoadNextWaveEnemies();
    FDynastyEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
    int32 TrySpawnWaveEnemies();
    bool ShouldKeepSpawnEnemies() const;

    UFUNCTION()
    void OnEnemyDestyoed(AActor* DestroyActor);

    UPROPERTY()
    TMap< TSoftClassPtr < AEnemyCharacter >, UClass* > PreLoadedEnemyClassMap;

    UPROPERTY()
    EDynastySurvialGameModeState CurrentSurvivalGameModeState;

    UPROPERTY()
    int32 CurrentSpawnedEnemiesCounter = 0;

    UPROPERTY()
    int32 TotalSpawnedEnemiesThisWaveCounter = 0;

    UPROPERTY()
    TArray<AActor*> TargetPointsArray;

    UPROPERTY(BlueprintCallable, BlueprintAssignable)
    FOnSurvialGameModeStateChangedDelegate OnSurvialGameModeStateChanged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
    UDataTable* EnemyWaveSpawnerDataTable;
	
    bool HasFinishedAllWaves() const;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
    int32 TotalWavesToSpawn;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
    int32 CurrentWaveCount = 1;

    UPROPERTY()
    float TimePassedSinceStart = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
    float SpawnNewWaveWaitTime = 5.f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
    float SpawnEnemiesDelayTime = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
    float WaveCompletedWaitTime = 5.f;


public:
    UFUNCTION(BlueprintCallable)
    void RegisterSpawnedEnemies(const TArray<AEnemyCharacter*>& InEnemiesToRegister);

};
