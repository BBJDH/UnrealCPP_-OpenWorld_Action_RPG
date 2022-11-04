
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHuman.generated.h"
/*
 ======================================

 ======================================
 */

DECLARE_MULTICAST_DELEGATE(FActionCall);

UCLASS()
class TPS_RPG_API ACHuman : public ACharacter
{
	GENERATED_BODY()

public:
	FActionCall JumpCall;


	//컴포넌트
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//커스텀 
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

public:
	ACHuman();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

private:

	//BindAction
	void OnJumpPressed();
	void OnJumpReleased();

	//BindAxis
	void OnMoveForward(float const InAxisValue);
	void OnMoveRight(float const InAxisValue);
	void HorizontalLook(float const InAxisValue);
	void VerticalLook(float const InAxisValue);




private:
	//내부 설정변수
	float const MAX_WALK_SPEED = 600;

private:
	//내부 함수
	void Asign();
	void Bind(UInputComponent  * const PlayerInputComponent);



};



