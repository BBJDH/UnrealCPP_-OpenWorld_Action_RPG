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
	Sight->PeripheralVisionAngleDegrees = 150;	//�νĹ��� ��
	Sight->SetMaxAge(2);	//2�� �� ������

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	
	Perception->ConfigureSense(*Sight);	//�þ߰���
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); 
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	//�Լ��� �����Ͱ� ����ȭ �Ǿ��־�� �ϹǷ� ���⼭ ���ε�
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Owner = Cast<ACHuman_AI>(InPawn);
	SetGenericTeamId(Owner->GetGenericTeamId());
	//BehaviorTree�� ������ �����带 AI��Ʈ�ѷ��� ������ ������Ʈ�� ���

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
	//� ������ ����� �������� (nullptr�� ��� ����)
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
