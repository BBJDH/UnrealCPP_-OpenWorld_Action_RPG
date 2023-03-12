#include "CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CAttachment.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
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

	OwnerWC->GetAttachment()->OnCollision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
	OwnerWC->GetAttachment()->OffCollision();
}

