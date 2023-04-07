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
	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNullUObject(Weapon);
	CheckNullUObject(Weapon->GetEquipment());

	Weapon->NotifyBeginEquip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNullUObject(Weapon);
	CheckNullUObject(Weapon->GetEquipment());

	Weapon->NotifyEndEquip();
}
