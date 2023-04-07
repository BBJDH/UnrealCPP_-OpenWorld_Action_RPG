#include "CAnimNotify_End_DoAction.h"

#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CDoActionComponent.h"

FString UCAnimNotify_End_DoAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_End_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNullUObject(weapon);
	CheckNullUObject(weapon->GetDoAction());

	weapon->GetDoAction()->End_DoAction();

	
}
