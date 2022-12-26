// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN/CAnimNotify_Dash.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Human/CHuman_Player.h"



FString UCAnimNotify_Dash::GetNotifyName_Implementation() const
{
	return "Make Dash Movement";
}

void UCAnimNotify_Dash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<ACHuman_Player>(MeshComp->GetOwner());

	CheckNull(Owner);

	if (Owner->GetCharacterMovement()->IsFalling())
		return;

	CheckNull(Owner->GetController());

	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	CheckNull(PlayerController);

	PlayerController->DisableInput(PlayerController);
	Owner->DashEvent();
	
}

