// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(Owner);
	
	Speed = Owner->GetVelocity().Size2D();


}
