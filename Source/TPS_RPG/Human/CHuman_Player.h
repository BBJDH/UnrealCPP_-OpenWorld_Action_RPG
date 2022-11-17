// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Human/CHuman.h"
#include "CHuman_Player.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API ACHuman_Player : public ACHuman
{
	GENERATED_BODY()

public:
	ACHuman_Player();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	virtual void Asign() override;
	void Bind(UInputComponent* const PlayerInputComponent);

	//TestKey
	void TestKeyBroadCast() ;

	void HorizontalLook(float const InAxisValue);
	void VerticalLook(float const InAxisValue);

public:
	//Test
	FActionCall TestKeyEvent;
};
