// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CHuman.h"
#include "CHuman_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
//#define LOG_UCFeetComponent 1

//void UCAnimInstance::ToggleIK()
//{
//	IsOnFeetIK = !IsOnFeetIK;
//	CLog::Print(IsOnFeetIK);
//}

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACharacter>(TryGetPawnOwner());

	CheckNull(Owner);
	//Cast<ACHuman>(Owner)->StartFall.AddUFunction(this, "StartInAir");
	//Cast<ACHuman>(Owner)->EndFall.AddUFunction(this, "EndInAir");

	//Test
	//Cast<ACHuman_Player>(Owner)->TestKeyEvent.AddUFunction(this,"ToggleIK");

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(Owner);
	CheckNull(weapon);

	weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(Owner);
	
	Speed = Owner->GetVelocity().Size2D();

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(Owner);

	CheckNull(feet);
	if (!!feet &&  Owner->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero())
	{
		IsOnFeetIK = true;
		FeetIKData = feet->GetData();
#if LOG_UCFeetComponent
		CLog::Print(FeetIKData.PelvisDistance, 11);
		CLog::Print(FeetIKData.LeftDistance, 12);
		CLog::Print(FeetIKData.RightDistance, 13);
		CLog::Print(FeetIKData.LeftRotation, 14);
		CLog::Print(FeetIKData.RightRotation, 15);

#endif
	}
	else
	{
		IsOnFeetIK = false;
	}
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCAnimInstance::StartInAir()
{
	InAir = true;
}

void UCAnimInstance::EndInAir()
{
	InAir = false;
}
