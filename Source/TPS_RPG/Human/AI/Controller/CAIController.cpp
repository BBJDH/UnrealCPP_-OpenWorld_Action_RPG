// Fill out your copyright notice in the Description page of Project Settings.


#include "CAIController.h"

#include <string>

#include "AIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Component/CAINormalBehaviorComponent.h"
#include "Human/CHuman_AI.h"
#include "Human/AI/Perception/CAIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	Perception = this->CreateDefaultSubobject<UCAIPerceptionComponent>("Perception");
	CHECK_NULL_UOBJECT(Perception);

	Blackboard = this->CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	CHECK_NULL_UOBJECT(Blackboard);

	Sight = this->CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	CHECK_NULL_UOBJECT(Sight);
	
	AINormalBehaviorComp = this->CreateDefaultSubobject<UCAINormalBehaviorComponent>("AIState");
	CHECK_NULL_UOBJECT(AINormalBehaviorComp);
	//AINormalBehaviorComp->SetBlackboard(Blackboard);

	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 180;	//�νĹ��� ��
	Sight->SetMaxAge(5);	//2�� �� ������

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	
	Perception->ConfigureSense(*Sight);	//�þ߰���
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); 
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	CHECK_NULL_UOBJECT(Perception);

	//�Լ��� �����Ͱ� ����ȭ �Ǿ��־�� �ϹǷ� ���⼭ ���ε�
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
	Perception->OnExpiredStimulus.AddUFunction(this,"BlackboardTargetUpdate");
}

void ACAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//BlackboardTargetUpdate();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerCharacter = Cast<ACHuman_AI>(InPawn);
	SetGenericTeamId(OwnerCharacter->GetGenericTeamId());
	//BehaviorTree�� ������ �����带 AI��Ʈ�ѷ��� ������ ������Ʈ�� ���

	CHECK_NULL_UOBJECT(OwnerCharacter->GetBehaviorTree());

	UseBlackboard(OwnerCharacter->GetBehaviorTree()->BlackboardAsset, Blackboard);

	RunBehaviorTree(OwnerCharacter->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

//�������� Target�� ���°�쿡�� ���⿡�� Target�� ����
void ACAIController::OnPerceptionUpdated(const FActorPerceptionUpdateInfo& UpdateInfo) 
{
	CHECK_NULL_UOBJECT(Perception);
	CHECK_NULL_UOBJECT(Blackboard);
	CHECK_TRUE(IsValid(Blackboard->GetValueAsObject(TargetName)) == true);
	
	UE_LOG(GameProject, Display, TEXT("OnPerceptionUpdated %s"), *OwnerCharacter->GetName());

	TArray<AActor*> DetectedActors;
	//� ������ ����� �������� (nullptr�� ��� ����)
	Perception->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), DetectedActors);
	
	for (AActor* Actor : DetectedActors)
	{
		if (IsHuman(Actor) == true)
		{
			ACHuman  *  Target = Cast<ACHuman>(Actor);
			Blackboard->SetValueAsObject(TargetName, Target);
			return;
		}
	}
	
}
//��ȿ�� Human�̶�� True ��ȯ;
bool ACAIController::IsHuman(AActor* InActor) const
{
	ACHuman const * const Target = Cast<ACHuman>(InActor);

	CHECK_TRUE_RESULT(IsValid(Target) == false,false);
	UCStateComponent * State =Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));
	CHECK_NULL_UOBJECT_RESULT(State,false);
	CHECK_TRUE_RESULT(State->IsDeadMode()==true,false);
	return true;
}

void ACAIController::BlackboardTargetUpdate() 
{
	TArray<AActor*> KnownPerceivedActors;
	Perception->GetKnownPerceivedActors(TSubclassOf<UAISense_Sight>(),KnownPerceivedActors);
	for (AActor * const Actor : KnownPerceivedActors )
	{
		if(IsHuman(Actor) == true)
		{
			ACHuman * PrevTarget = Cast<ACHuman>(Blackboard->GetValueAsObject(TargetName));
			if(PrevTarget == Actor)
			{
				return;
			}
		}
	}

	//TargetLost
	StopMovement();
	ACHuman * NewTarget = nullptr;
	for (AActor * const Actor : KnownPerceivedActors )
	{
		if(IsHuman(Actor) == true)
		{
			NewTarget = Cast<ACHuman>(Actor);
			break;
		}
	}
	Blackboard->SetValueAsObject("Target", NewTarget);
}
