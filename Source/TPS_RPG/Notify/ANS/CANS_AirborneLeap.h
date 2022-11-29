// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CANS_AirborneLeap.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCANS_AirborneLeap : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		bool IsKeyPressed=false;
	UPROPERTY(EditAnywhere)
		class APlayerController* PlayerController;
	UPROPERTY(EditAnywhere)
		class ACHuman* Player;
public:

	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
