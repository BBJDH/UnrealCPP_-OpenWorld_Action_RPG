// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CHuman_Player.h"
#include "Global.h"
#include "Component/CMoveComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CZoomComponent.h"

#include "GameFramework/SpringArmComponent.h"


ACHuman_Player::ACHuman_Player():ACHuman()
{
	CHECK_NULL_UOBJECT(SpringArm);
	
	//CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	Zoom = this->CreateDefaultSubobject<UCZoomComponent>("Zoom");
	CHECK_NULL_UOBJECT(Zoom);


	//SpringArm Setting
	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->SocketOffset = FVector(0, 60, 0);
	SpringArm->TargetArmLength = 600;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5;
	SpringArm->CameraRotationLagSpeed = 2;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bUsePawnControlRotation = true;


	//Jump Setting
	this->JumpMaxCount = 2;
	this->JumpMaxHoldTime = 0.2f;
}

void ACHuman_Player::BeginPlay()
{
	Super::BeginPlay();
}


void ACHuman_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	CHECK_NULL_UOBJECT(PlayerInputComponent);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Bind(PlayerInputComponent);
}

void ACHuman_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//DashTimeline.TickTimeline(DeltaSeconds);
}

//플레이어 입력 이벤트
void ACHuman_Player::Bind(UInputComponent* const PlayerInputComponent)
{
	CHECK_NULL_UOBJECT(PlayerInputComponent);
	CHECK_NULL_UOBJECT(Weapon);
	CHECK_NULL_UOBJECT(Move);
	CHECK_NULL_UOBJECT(Zoom);
	
	//Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACHuman_Player::OnJumpPressed);

	PlayerInputComponent->BindAction("GreatSword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetGreatSwordMode);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);
	PlayerInputComponent->BindAction("UpperAttack", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoUpperAction);
	PlayerInputComponent->BindAction("R_Skill", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Do_R_Action);
	PlayerInputComponent->BindAction("Sprint/Avoid", EInputEvent::IE_Pressed, Move, &UCMoveComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint/Avoid", EInputEvent::IE_Released, Move, &UCMoveComponent::OffSprint);
	PlayerInputComponent->BindAction("Sprint/Avoid", EInputEvent::IE_DoubleClick, Move, &UCMoveComponent::OffSprint);


	//Test
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Pressed, this, &ACHuman_Player::TestKeyFunctionPressed);
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Released, this, &ACHuman_Player::TestKeyFunctionReleased);

	//Axis
	PlayerInputComponent->BindAxis("MoveForward", Move, &UCMoveComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Move, &UCMoveComponent::OnMoveRight);

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACHuman_Player::HorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACHuman_Player::VerticalLook);

	PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetZoomValue);
}

void ACHuman_Player::TestKeyFunctionPressed()
{
	FString StrToString = "TestKey Pressed";
	UE_LOG(GameProject, Display, TEXT("%s"), *StrToString);
}

void ACHuman_Player::TestKeyFunctionReleased()
{
	FString StrToString = "TestKey Released";
	UE_LOG(GameProject, Display, TEXT("%s"), *StrToString);
}

void ACHuman_Player::HorizontalLook(float const InAxisValue)
{
	AddControllerYawInput(InAxisValue);
}

void ACHuman_Player::VerticalLook(float const InAxisValue)
{
	AddControllerPitchInput(InAxisValue);
}

//void ACHuman_Player::NotifyDashEvent()
//{
//	CheckNull(CurveFloat);
//	DashSetup(); 
//	DisableInput(Cast<APlayerController>(GetController()));
//
//	FOnTimelineFloat TimelineProgress;
//	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
//	DashTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
//
//	FOnTimelineEvent TimelineFinishedCallback;
//	TimelineFinishedCallback.BindUFunction(this, FName("TimelineStop"));
//	DashTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
//	
//	DashTimeline.PlayFromStart();
//}
//
//void ACHuman_Player::TimelineProgress(float const Axis)
//{
//	AddMovementInput(Axis*this->GetActorForwardVector());
//}
//
//void ACHuman_Player::TimelineStop()
//{
//	EnableInput(Cast<APlayerController>(GetController()));
//	this->GetCharacterMovement()->MaxWalkSpeed = BackUp_MaxWalkSpeed;
//	this->GetCharacterMovement()->MaxAcceleration = BackUp_MaxAcceleration;
//	this->GetCharacterMovement()->StopMovementImmediately();
//}
//
//void ACHuman_Player::DashSetup()
//{
//	GetCharacterMovement()->MaxWalkSpeed = 20000.0f;
//	GetCharacterMovement()->MaxAcceleration = 1000000000.0f;
//}
//
