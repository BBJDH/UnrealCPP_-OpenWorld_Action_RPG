#include "CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CDoActionComponent.h"
#include "Weapon/DoActions/CDoComboActionComponent.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoComboActionComponent* combo = Cast<UCDoComboActionComponent>(weapon->GetDoAction());
	CheckNull(combo);

	combo->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoComboActionComponent* combo = Cast<UCDoComboActionComponent>(weapon->GetDoAction());
	CheckNull(combo);

	combo->DisableCombo();
}
