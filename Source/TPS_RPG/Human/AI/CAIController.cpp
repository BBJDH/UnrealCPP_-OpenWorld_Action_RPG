// Fill out your copyright notice in the Description page of Project Settings.


#include "CAIController.h"

#include "AIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Component/CAIStateComponent.h"
#include "Human/CHuman_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 150;	//인식범위 각
	Sight->SetMaxAge(2);	//2초 후 잊혀짐

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	
	Perception->ConfigureSense(*Sight);	//시야감지
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); 
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	//함수의 포인터가 직렬화 되어있어야 하므로 여기서 바인드
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Owner = Cast<ACHuman_AI>(InPawn);
	SetGenericTeamId(Owner->GetGenericTeamId());
	//BehaviorTree에 설정된 블랙보드를 AI컨트롤러의 블랙보드 컴포넌트에 등록

	CheckNull(Owner->GetBehaviorTree());

	UseBlackboard(Owner->GetBehaviorTree()->BlackboardAsset, Blackboard);

	AIState = CHelpers::GetComponent<UCAIStateComponent>(Owner);
	//CheckNull(AIState);
	AIState->SetBlackboard(Blackboard);

	RunBehaviorTree(Owner->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	//어떤 감지로 뜬것을 가져올지 (nullptr은 모든 감지)
	Perception->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), actors);

	ACHuman* Target = nullptr;
	for (AActor* actor : actors)
	{
		Target = Cast<ACHuman>(actor);
		if (!!Target)
			break;
	}
	Blackboard->SetValueAsObject("Target", Target);
}
