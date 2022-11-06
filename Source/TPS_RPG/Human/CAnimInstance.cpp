// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CHuman.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACharacter>(TryGetPawnOwner());

	CheckNull(Owner);
	Cast<ACHuman>(Owner)->StartFall.AddUFunction(this, "StartInAir");
	Cast<ACHuman>(Owner)->EndFall.AddUFunction(this, "EndInAir");
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(Owner);
	
	Speed = Owner->GetVelocity().Size2D();

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(Owner);

	CheckNull(feet);
	//if (!!feet &&  Owner->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero())
	//{
	//	IsOnFeetIK = true;
	//	FeetIKData = feet->GetData();
	//}
	//else
	//	IsOnFeetIK = false;
}

void UCAnimInstance::StartInAir()
{
	InAir = true;
}

void UCAnimInstance::EndInAir()
{
	InAir = false;
}
