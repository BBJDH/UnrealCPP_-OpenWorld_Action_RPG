// Fill out your copyright notice in the Description page of Project Settings.


#include "CAIController.h"

#include "AIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Component/CAINormalBehaviorComponent.h"
#include "Human/CHuman_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	Perception = this->CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	CheckNullUObject(Perception);

	Blackboard = this->CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	CheckNullUObject(Blackboard);

	Sight = this->CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	CheckNullUObject(Sight);
	
	AINormalBehaviorComp = this->CreateDefaultSubobject<UCAINormalBehaviorComponent>("AIState");
	CheckNullUObject(AINormalBehaviorComp);
	//AINormalBehaviorComp->SetBlackboard(Blackboard);

	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 150;	//인식범위 각
	Sight->SetMaxAge(5);	//2초 후 잊혀짐

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	
	Perception->ConfigureSense(*Sight);	//시야감지
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); 
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	CheckNullUObject(Perception);

	//함수의 포인터가 직렬화 되어있어야 하므로 여기서 바인드
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerCharacter = Cast<ACHuman_AI>(InPawn);
	SetGenericTeamId(OwnerCharacter->GetGenericTeamId());
	//BehaviorTree에 설정된 블랙보드를 AI컨트롤러의 블랙보드 컴포넌트에 등록

	CheckNullUObject(OwnerCharacter->GetBehaviorTree());

	UseBlackboard(OwnerCharacter->GetBehaviorTree()->BlackboardAsset, Blackboard);

	RunBehaviorTree(OwnerCharacter->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const FActorPerceptionUpdateInfo& UpdatedInfo)
{
	CheckNullUObject(Perception);
	CheckNullUObject(Blackboard);

	UE_LOG(GameProject, Display, TEXT("OnPerceptionUpdated"));
	if(UpdatedInfo.Target.IsValid()==false)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		return;
	}
	// ACHuman* Target = Cast<ACHuman>(Blackboard->GetValueAsObject(TargetName));
	//
	// if(IsValid(Target)==true)
	// {
	// 	return;
	// }
	ACHuman* Target = nullptr;
	TArray<AActor*> DetectedActors;
	//어떤 감지로 뜬것을 가져올지 (nullptr은 모든 감지)
	Perception->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), DetectedActors);
	for (AActor* actor : DetectedActors)
	{
		Target = Cast<ACHuman>(actor);
		if (Target != nullptr)
			break;
	}

	//UE_LOG(GameProject, Warning, TEXT("Target Is : %s"), Target);
	Blackboard->SetValueAsObject("Target", Target);
}
