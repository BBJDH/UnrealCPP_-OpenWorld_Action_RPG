// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CANS_UnPossess.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCANS_UnPossess : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	class ACharacter* Owner;
	class AController* Controller;
	
};
