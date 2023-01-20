// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/AI/CBTService_Melee.h"

#include "Global.h"
#include "Human/CHuman_AI.h"
#include "Human/AI/CAIController.h"


UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACHuman_AI* ai = Cast<ACHuman_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	//UCAIStateComponent* aiState = CHelpers::GetComponent<UCAIStateComponent>(ai);

	//if (state->IsHittedMode())
	//{
	//	aiState->SetHittedMode();
	//	return;
	//}

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

	//aiState->SetApproachMode();
}
