// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()

	
public:
	UCBTService_Melee();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 150;
	UPROPERTY(EditAnywhere,Category = "NameOfBlackBoardKey")
		FName KeyNameOfEnemy = "Target";

	
};
