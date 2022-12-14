// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN/CAnimNotify_Dash.h"
#include "Global.h"


FString UCAnimNotify_Dash::GetNotifyName_Implementation() const
{
	return "Make Dash Movement";
}

void UCAnimNotify_Dash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

}
