// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Dead.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCAnimNotify_Dead : public UAnimNotify
{
	GENERATED_BODY()


public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
