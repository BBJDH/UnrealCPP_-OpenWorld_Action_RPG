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
	CHECK_NULL_UOBJECT(MeshComp);
	CHECK_NULL_UOBJECT(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CHECK_NULL_UOBJECT(WeaponComponent);

	if(WeaponComponent->IsUnarmedMode()==false)
	{
		CHECK_NULL_UOBJECT(WeaponComponent->GetDoAction());

		WeaponComponent->GetDoAction()->End_DoAction();
	}
}
