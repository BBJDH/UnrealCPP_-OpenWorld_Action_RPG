
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

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void Asign() override;
	void Bind(UInputComponent* const PlayerInputComponent);

	//TestKey
	void TestKeyBroadCast() ;

	void HorizontalLook(float const InAxisValue);
	void VerticalLook(float const InAxisValue);


	//TODO : Dash Ã³¸®Áß
public:
	//Test
	FActionCall TestKeyEvent;

	//Dash Timeline
	FTimeline DashTimeline;

	UPROPERTY(EditAnywhere, Category = "Dash")
		class UCurveFloat* CurveFloat;
	void DashEvent();
	UFUNCTION()
		void TimelineProgress(float const Axis);
	UFUNCTION()
		void TimelineStop();
	void DashSetup();

private:
	float const  BackUp_MaxWalkSpeed = 600;
	float const BackUp_MaxAcceleration = 2048.0f;
};
