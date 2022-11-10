#include "CAnimNotify_End_DoAction.h"

#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapon/CDoAction.h"

FString UCAnimNotify_End_DoAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_End_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	weapon->GetDoAction()->End_DoAction();

	
}
