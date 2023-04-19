#include "Weapon/CEquipment.h"

#include "CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCEquipment::Equip(ACAttachment * InAttachment)
{
	CHECK_NULL_UOBJECT(InAttachment)
	State->SetEquipMode();


	if (Data.Montage != nullptr)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRatio);
	}
	else
	{
		Begin_Equip(InAttachment);
		End_Equip();
	}
}

void UCEquipment::Begin_Equip(class ACAttachment* InAttachment)
{
	CHECK_NULL_UOBJECT(InAttachment)

	bBeginEquip = true;
	InAttachment->OnBeginEquip();
	//if (OnBeginEquip.IsBound())
	//	OnBeginEquip.Broadcast();
}

void UCEquipment::End_Equip()
{
	bBeginEquip = false;
	bEquipped = true;

	State->SetIdleMode();
}

void UCEquipment::Unequip(ACAttachment* InAttachment)
{
	CHECK_NULL_UOBJECT(InAttachment)

	bEquipped = false;

	InAttachment->OnUnequip();
	//if (OnUnequip.IsBound())
	//	OnUnequip.Broadcast();
}