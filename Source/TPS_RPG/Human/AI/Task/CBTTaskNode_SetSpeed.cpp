// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/AI/Task/CBTTaskNode_SetSpeed.h"

#include "Human/AI/Controller/CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Human/CHuman_AI.h"

UCBTTaskNode_SetSpeed::UCBTTaskNode_SetSpeed()
{
	NodeName = "SetSpeed";
}

EBTNodeResult::Type UCBTTaskNode_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetOwner() == nullptr)
		return EBTNodeResult::Failed;

	ACAIController* controller =  Cast<ACAIController> (OwnerComp.GetOwner());

	if (controller->GetPawn() == nullptr)
		return EBTNodeResult::Failed;

	ACHuman_AI* OwnerCharacter = Cast<ACHuman_AI>(controller->GetPawn());

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;


	return EBTNodeResult::Succeeded;
}
