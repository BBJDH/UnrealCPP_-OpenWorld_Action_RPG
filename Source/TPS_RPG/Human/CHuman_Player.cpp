// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CHuman_Player.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Component/CZoomComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CWeaponStructures.h"

DEFINE_LOG_CATEGORY_STATIC(GameProject, Display, All)

ACHuman_Player::ACHuman_Player():ACHuman()
{
	Asign();
}


void ACHuman_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Bind(PlayerInputComponent);
}

void ACHuman_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DashTimeline.TickTimeline(DeltaSeconds);
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
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACHuman_Player::OnJumpReleased);

	PlayerInputComponent->BindAction("GreatSword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetGreatSwordMode);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);
	PlayerInputComponent->BindAction("UpperAttack", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoUpperAction);
	PlayerInputComponent->BindAction("R_Skill", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Do_R_Action);

	//Test
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Pressed, this, &ACHuman_Player::TestKeyFunctionPressed);
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Released, this, &ACHuman_Player::TestKeyFunctionReleased);
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

void ACHuman_Player::TestKeyFunctionPressed()
{
	FString StrToString = "TestKey Pressed";
	UE_LOG(GameProject, Display, TEXT("%s"), *StrToString);

	//GetCapsuleComponent()->SetWorldScale3D(FVector(1,2,1));
	Weapon->GetAttachment()->OnAirCollision();
}

void ACHuman_Player::TestKeyFunctionReleased()
{
	FString StrToString = "TestKey Released";
	UE_LOG(GameProject, Display, TEXT("%s"), *StrToString);

	//GetCapsuleComponent()->SetWorldScale3D(FVector(1, 1, 1));
	Weapon->GetAttachment()->OffAirCollision();

}

void ACHuman_Player::HorizontalLook(float const InAxisValue)
{
	AddControllerYawInput(InAxisValue);
}

void ACHuman_Player::VerticalLook(float const InAxisValue)
{
	AddControllerPitchInput(InAxisValue);
}

void ACHuman_Player::DashEvent()
{
	CheckNull(CurveFloat);
	DashSetup(); 
	DisableInput(Cast<APlayerController>(GetController()));

	FOnTimelineFloat timelineProgress;
	timelineProgress.BindUFunction(this, FName("TimelineProgress"));
	DashTimeline.AddInterpFloat(CurveFloat, timelineProgress);

	FOnTimelineEvent timelineFinishedCallback;
	timelineFinishedCallback.BindUFunction(this, FName("TimelineStop"));
	DashTimeline.SetTimelineFinishedFunc(timelineFinishedCallback);
	
	DashTimeline.PlayFromStart();
}

void ACHuman_Player::TimelineProgress(float const Axis)
{
	AddMovementInput(Axis*this->GetActorForwardVector());
}

void ACHuman_Player::TimelineStop()
{
	EnableInput(Cast<APlayerController>(GetController()));
	this->GetCharacterMovement()->MaxWalkSpeed = BackUp_MaxWalkSpeed;
	this->GetCharacterMovement()->MaxAcceleration = BackUp_MaxAcceleration;
	this->GetCharacterMovement()->StopMovementImmediately();
}

void ACHuman_Player::DashSetup()
{
	GetCharacterMovement()->MaxWalkSpeed = 20000.0f;
	GetCharacterMovement()->MaxAcceleration = 1000000000.0f;
}

