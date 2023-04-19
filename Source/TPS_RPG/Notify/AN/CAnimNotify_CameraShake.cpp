// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN/CAnimNotify_CameraShake.h"
#include "Global.h"
#include "MatineeCameraShake.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CHECK_NULL_UOBJECT(MeshComp);
	CHECK_NULL_UOBJECT(MeshComp->GetOwner());
	CHECK_NULL_UOBJECT(CameraShakeClass);

	ACharacter* character = MeshComp->GetOwner<ACharacter>();
	CHECK_NULL_UOBJECT(character);

	APlayerController* controller = character->GetController<APlayerController>();
	CHECK_NULL_UOBJECT(controller);

	controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
