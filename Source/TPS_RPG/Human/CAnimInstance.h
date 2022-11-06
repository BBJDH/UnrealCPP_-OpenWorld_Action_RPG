// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/CFeetComponent.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool IsOnFeetIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	FFeetData FeetIKData;

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//내부 함수

private:
	//직렬화 해야 델리게이트에서 찾기 가능
	UFUNCTION()
	void StartInAir();
	UFUNCTION()
	void EndInAir();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet IK")
		bool InAir;

private: 
	ACharacter* Owner;

};
