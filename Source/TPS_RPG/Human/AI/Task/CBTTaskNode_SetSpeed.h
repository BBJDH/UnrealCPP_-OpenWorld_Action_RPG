// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCBTTaskNode_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_SetSpeed();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
