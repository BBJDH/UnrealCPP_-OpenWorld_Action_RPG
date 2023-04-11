// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

#include "CAIController.generated.h"

/**
 * 
 */

UCLASS()
class TPS_RPG_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

public:
	ACAIController();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const FActorPerceptionUpdateInfo& UpdatedActors);
	
private:
	UPROPERTY(VisibleDefaultsOnly)

	class ACHuman_AI* OwnerCharacter;

	UPROPERTY(VisibleDefaultsOnly)

	class UAISenseConfig_Sight* Sight;

	UPROPERTY(VisibleDefaultsOnly)

	class UCAINormalBehaviorComponent* AINormalBehaviorComp;

	UPROPERTY(EditAnywhere,Category = "NameOfBlackBoardKey")
	FName TargetName = "Target";
};
