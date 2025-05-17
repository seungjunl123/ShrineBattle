// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/DynastyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

ADynastyAIController::ADynastyAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; // 적에 대해서만 인식
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdate);

	SetGenericTeamId(FGenericTeamId(1));
}

void ADynastyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

ETeamAttitude::Type ADynastyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void ADynastyAIController::OnEnemyPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			UE_LOG(LogTemp, Display, TEXT("TargetActor is %s"),*Actor->GetActorNameOrLabel());
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}

