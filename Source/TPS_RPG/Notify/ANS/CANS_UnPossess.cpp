// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/CANS_UnPossess.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

FString UCANS_UnPossess::GetNotifyName_Implementation() const
{
	return "UnPossess";
}

void UCANS_UnPossess::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CHECK_NULL_UOBJECT(MeshComp);
	Owner = Cast<ACharacter>(MeshComp->GetOwner());
	CHECK_NULL_UOBJECT(Owner);
	Controller = Owner->GetController();
	CHECK_NULL_UOBJECT(Controller);
	Owner->DisableInput(Cast<APlayerController>(Controller));
}

void UCANS_UnPossess::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CHECK_NULL_UOBJECT(Owner);
	CHECK_NULL_UOBJECT(Controller);
	Owner->EnableInput(Cast<APlayerController>(Controller));
}
