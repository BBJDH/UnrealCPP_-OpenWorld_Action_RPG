
#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	void DashEvent();


protected:
	void Bind(UInputComponent* const PlayerInputComponent);

	void HorizontalLook(float const InAxisValue);
	void VerticalLook(float const InAxisValue);


	//TestKey
	void TestKeyFunctionPressed();
	void TestKeyFunctionReleased();

private:
	 void Asign();

	void DashSetup();

	UFUNCTION()
		void TimelineProgress(float const Axis);
	UFUNCTION()
		void TimelineStop();



public:

	//Dash Timeline
	FTimeline DashTimeline;

	UPROPERTY(EditAnywhere, Category = "Dash")
		class UCurveFloat* CurveFloat;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;


private:
	float const  BackUp_MaxWalkSpeed = 600;
	float const BackUp_MaxAcceleration = 2048.0f;
};
