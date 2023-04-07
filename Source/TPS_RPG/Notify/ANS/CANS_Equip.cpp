// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/CANS_Equip.h"
#include "Global.h"
#include "Human/CHuman.h"

FString UCANS_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCANS_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());
	ACHuman* human = Cast<ACHuman>(MeshComp->GetOwner());
	CheckNullUObject(human);

}

void UCANS_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());
	ACHuman* human = Cast<ACHuman>(MeshComp->GetOwner());
	CheckNullUObject(human);
}