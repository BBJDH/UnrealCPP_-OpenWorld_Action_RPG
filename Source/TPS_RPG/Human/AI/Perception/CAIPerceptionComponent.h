// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CAIPerceptionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FExpiredStimulus);

UCLASS()
class TPS_RPG_API UCAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

	virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;

public:
	FExpiredStimulus OnExpiredStimulus;
};
