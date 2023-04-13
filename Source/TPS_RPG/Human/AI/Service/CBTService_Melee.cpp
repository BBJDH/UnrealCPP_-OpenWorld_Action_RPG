// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/AI/Service/CBTService_Melee.h"

#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/CAINormalBehaviorComponent.h"
#include "Component/CWeaponComponent.h"
#include "Human/CHuman_AI.h"
#include "Human/AI/Controller/CAIController.h"


UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
	Interval = 0.01f;
	RandomDeviation = 0.0f;
}

//���, ����, �������·� �б� ó��
void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController const *const AIController = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullUObject(AIController);

	ACHuman_AI const *const OwnerCharacter = Cast<ACHuman_AI>(AIController->GetPawn());
	CheckNullUObject(OwnerCharacter);

	UCStateComponent const *const StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNullUObject(StateComponent);

	UCWeaponComponent const *const WeaponComponent = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullUObject(WeaponComponent);

	
	// CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	UCAINormalBehaviorComponent const *const AINormalBehavior = Cast<UCAINormalBehaviorComponent>(AIController->GetComponentByClass(UCAINormalBehaviorComponent::StaticClass()));
	//UCAINormalBehaviorComponent* AINormalBehavior = CHelpers::GetComponent<UCAINormalBehaviorComponent>(OwnerCharacter);
	CheckNullUObject(AINormalBehavior);

	CheckTrue(AINormalBehavior->IsStopAIMode());

	ACHuman * TargetInBlackBoard = Cast<ACHuman>(AIController->GetBlackboardComponent()->GetValueAsObject(KeyNameOfEnemy));

	if(IsValid(TargetInBlackBoard) == true)
	{
		AINormalBehavior->SetBattleMode();
		return;
	}

	AINormalBehavior->SetPatrolMode();





	//ACPlayer* player = aiState->GetTargetPlayer();
	//if (player == nullptr)
	//{
	//	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	//	if (!!patrol)
	//	{
	//		aiState->SetPatrolMode();
	//		return;
	//	}
	//	aiState->SetWaitMode();
	//	return;
	//}

	//float distance = ai->GetDistanceTo(player);
	//if (distance < ActionRange)
	//{
	//	aiState->SetActionMode();
	//	return;
	//}

}
