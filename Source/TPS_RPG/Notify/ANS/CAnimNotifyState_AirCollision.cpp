// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/CAnimNotifyState_AirCollision.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CAttachment.h"

FString UCAnimNotifyState_AirCollision::GetNotifyName_Implementation() const
{
	return "AirCollision";
}

void UCAnimNotifyState_AirCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(IsValid(MeshComp) == false)
	{
		return;
	}
	if(IsValid(MeshComp->GetOwner()) ==false)
	{
		return;
	}

	UCWeaponComponent * OwnerWC = Cast<UCWeaponComponent>( MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if(IsValid(OwnerWC)== false)
	{
		return;
	}
	if (IsValid(OwnerWC->GetAttachment()) == false)
	{
		return;
	}
	OwnerWC->GetAttachment()->OnAirCollision();
}

void UCAnimNotifyState_AirCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (IsValid(MeshComp) == false)
	{
		return;
	}
	if (IsValid(MeshComp->GetOwner()) == false)
	{
		return;
	}

	UCWeaponComponent* OwnerWC = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (IsValid(OwnerWC) == false)
	{
		return;
	}
	if (IsValid(OwnerWC->GetAttachment()) == false)
	{
		return;
	}
	OwnerWC->GetAttachment()->OffAirCollision();
}
