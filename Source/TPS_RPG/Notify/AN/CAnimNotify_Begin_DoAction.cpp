#include "CAnimNotify_Begin_DoAction.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CDoActionComponent.h"

FString UCAnimNotify_Begin_DoAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";
}

void UCAnimNotify_Begin_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	//UCWeaponComponent* WeaponComponent = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullUObject(WeaponComponent);
	CheckNullUObject(WeaponComponent->GetDoAction());

	WeaponComponent->GetDoAction()->Begin_DoAction();
}