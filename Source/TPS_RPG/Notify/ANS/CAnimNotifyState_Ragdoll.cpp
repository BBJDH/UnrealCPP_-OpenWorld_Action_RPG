// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/CAnimNotifyState_Ragdoll.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

FString UCAnimNotifyState_Ragdoll::GetNotifyName_Implementation() const
{
	return "MakeRagdoll";
}

void UCAnimNotifyState_Ragdoll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(IsValid(MeshComp)==false)
	{
		return;
	}
	if(IsValid(MeshComp->GetOwner())==false)
	{
		return;
	}

	class ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());

	if(IsValid(OwnerCharacter)==false)
	{
		return;
	}
	OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName("Ragdoll");
}

void UCAnimNotifyState_Ragdoll::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	class ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());

	if (IsValid(OwnerCharacter) == false)
	{
		return;
	}
	OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
}
