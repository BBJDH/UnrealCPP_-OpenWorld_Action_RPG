// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN/CAnimNotify_EndOfHitMontage.h"
#include "Component/CStateComponent.h"

FString UCAnimNotify_EndOfHitMontage::GetNotifyName_Implementation() const
{
	return "EndOfHitMontage";
}

void UCAnimNotify_EndOfHitMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(IsValid(MeshComp)==false)
	{
		return;
	}
	if(IsValid(MeshComp->GetOwner())==false)
	{
		return;
	}

	UCStateComponent* StateComponentOfOwner = 
		Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));

	if(IsValid(StateComponentOfOwner)==false)
	{
		return;
	}
	StateComponentOfOwner->SetIdleMode();
}
