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
	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	//UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullUObject(WeaponComponent);
	CheckNullUObject(WeaponComponent->GetDoAction());

	UCDoComboActionComponent* ComboComponent = Cast<UCDoComboActionComponent>(WeaponComponent->GetDoAction());
	CheckNullUObject(ComboComponent);

	ComboComponent->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullUObject(WeaponComponent);
	CheckNullUObject(WeaponComponent->GetDoAction());

	UCDoComboActionComponent* ComboComponent = Cast<UCDoComboActionComponent>(WeaponComponent->GetDoAction());
	CheckNullUObject(ComboComponent);

	ComboComponent->DisableCombo();
}
