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
	//CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	//CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	Perception = this->CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	CheckNullUObject(Perception);

	Blackboard = this->CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	CheckNullUObject(Blackboard);

	Sight = this->CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	CheckNullUObject(Sight);

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
	CheckNullUObject(Perception);

	//�Լ��� �����Ͱ� ����ȭ �Ǿ��־�� �ϹǷ� ���⼭ ���ε�
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerCharacter = Cast<ACHuman_AI>(InPawn);
	SetGenericTeamId(OwnerCharacter->GetGenericTeamId());
	//BehaviorTree�� ������ �����带 AI��Ʈ�ѷ��� ������ ������Ʈ�� ���

	CheckNullUObject(OwnerCharacter->GetBehaviorTree());

	UseBlackboard(OwnerCharacter->GetBehaviorTree()->BlackboardAsset, Blackboard);

	AIState = CHelpers::GetComponent<UCAIStateComponent>(OwnerCharacter);
	CheckNullUObject(AIState);
	AIState->SetBlackboard(Blackboard);

	RunBehaviorTree(OwnerCharacter->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckNullUObject(Perception);

	TArray<AActor*> DetectedActors;
	//� ������ ����� �������� (nullptr�� ��� ����)
	Perception->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), DetectedActors);

	ACHuman* Target = nullptr;
	for (AActor* actor : DetectedActors)
	{
		Target = Cast<ACHuman>(actor);
		if (Target != nullptr)
			break;
	}

	CheckNullUObject(Blackboard);
	Blackboard->SetValueAsObject("Target", Target);
}
