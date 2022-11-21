// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CHuman_Player.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Component/CZoomComponent.h"

#include "GameFramework/SpringArmComponent.h"

ACHuman_Player::ACHuman_Player():ACHuman()
{
	Asign();
}

void ACHuman_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Bind(PlayerInputComponent);
}

void ACHuman_Player::Asign()
{

	/*
	======================================
				SpringArm Setting
	======================================
	*/


	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->SocketOffset = FVector(0, 60, 0);
	SpringArm->TargetArmLength = 600;
	SpringArm->bEnableCameraLag = true;		//ī�޶� �ε巴�� ���������
	SpringArm->CameraLagSpeed = 5;
	SpringArm->CameraRotationLagSpeed = 2;
	SpringArm->bDoCollisionTest = true;	//�������Ͽ� �ɸ��� ���ư�����
	SpringArm->bUsePawnControlRotation = true;	//Pawn���� ȸ������

/*
======================================
			Jump Setting
======================================
*/
	this->JumpMaxCount = 2;
	this->JumpMaxHoldTime = 0.2f;
}

void ACHuman_Player::Bind(UInputComponent* const PlayerInputComponent)
{
	CheckNull(PlayerInputComponent);

	//Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACHuman_Player::OnJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACHuman_Player::OnJumpReleased);

	PlayerInputComponent->BindAction("GreatSword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetGreatSwordMode);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);


	//Test
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Pressed, this, &ACHuman_Player::TestKeyBroadCast);


	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACHuman_Player::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACHuman_Player::OnMoveRight);
	//���� ������ �Է¿��� ���ǵ� �̸��� �̺�Ʈ ���ε�
	//����ȭ ���� �ʾƵ� �ȴ� �Լ� ������ ���ε�
	//BindAxis�� ���ڷ� float �ϳ��� �޾ƾ� �Ѵ�

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACHuman_Player::HorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACHuman_Player::VerticalLook);

	PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetZoomValue);
}

void ACHuman_Player::TestKeyBroadCast() 
{
	if (TestKeyEvent.IsBound())
		TestKeyEvent.Broadcast();
}

void ACHuman_Player::HorizontalLook(float const InAxisValue)
{
	AddControllerYawInput(InAxisValue);
}

void ACHuman_Player::VerticalLook(float const InAxisValue)
{
	AddControllerPitchInput(InAxisValue);

}
