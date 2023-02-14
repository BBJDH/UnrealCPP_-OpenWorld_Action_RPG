// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/CFeetComponent.h"
#include "Component/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:


	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	//내부 함수
	//직렬화 해야 델리게이트에서 찾기 가능
	UFUNCTION()
	void StartInAir();
	UFUNCTION()
	void EndInAir();


	//Test용
	//UFUNCTION()
	//void ToggleIK();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet IK")
		bool InAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsOnFeetIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		FFeetData FeetIKData;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
		EWeaponType WeaponType;

private: 
	ACharacter* Owner;

};
