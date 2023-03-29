// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN/CAnimNotify_Dead.h"
#include "Global.h"
#include "Human/CHuman.h"

FString UCAnimNotify_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCAnimNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	ACHuman* OwnerHuman = Cast<ACHuman>(MeshComp->GetOwner());
	CheckNull(OwnerHuman);
	OwnerHuman->NotifyDead();
}
