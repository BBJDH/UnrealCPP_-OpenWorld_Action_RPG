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

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullUObject(WeaponComponent);

	if(WeaponComponent->IsUnarmedMode()==false)
	{
		CheckNullUObject(WeaponComponent->GetDoAction());

		WeaponComponent->GetDoAction()->End_DoAction();
	}
}
