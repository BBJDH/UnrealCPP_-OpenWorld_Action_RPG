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
	CHECK_NULL_UOBJECT(MeshComp);
	CHECK_NULL_UOBJECT(MeshComp->GetOwner());

	//UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CHECK_NULL_UOBJECT(WeaponComponent);
	CHECK_NULL_UOBJECT(WeaponComponent->GetDoAction());

	UCDoComboActionComponent* ComboComponent = Cast<UCDoComboActionComponent>(WeaponComponent->GetDoAction());
	CHECK_NULL_UOBJECT(ComboComponent);

	ComboComponent->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CHECK_NULL_UOBJECT(MeshComp);
	CHECK_NULL_UOBJECT(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CHECK_NULL_UOBJECT(WeaponComponent);
	CHECK_NULL_UOBJECT(WeaponComponent->GetDoAction());

	UCDoComboActionComponent* ComboComponent = Cast<UCDoComboActionComponent>(WeaponComponent->GetDoAction());
	CHECK_NULL_UOBJECT(ComboComponent);

	ComboComponent->DisableCombo();
}
