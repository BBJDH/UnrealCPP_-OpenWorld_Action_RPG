#include "CAnimNotifyState_Equip.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CEquipment.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CHECK_NULL_UOBJECT(MeshComp);
	CHECK_NULL_UOBJECT(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CHECK_NULL_UOBJECT(WeaponComponent);
	CHECK_NULL_UOBJECT(WeaponComponent->GetEquipment());

	WeaponComponent->NotifyBeginEquip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CHECK_NULL_UOBJECT(MeshComp);
	CHECK_NULL_UOBJECT(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CHECK_NULL_UOBJECT(WeaponComponent);
	CHECK_NULL_UOBJECT(WeaponComponent->GetEquipment());

	WeaponComponent->NotifyEndEquip();
}
