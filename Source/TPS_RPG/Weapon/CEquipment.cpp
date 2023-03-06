#include "Weapon/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void UCEquipment::Equip()
{
	State->SetEquipMode();


	if (Data.Montage != nullptr)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRatio);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}
}

void UCEquipment::Begin_Equip()
{
	bBeginEquip = true;

	if (OnBeginEquip.IsBound())
		OnBeginEquip.Broadcast();
}

void UCEquipment::End_Equip()
{
	bBeginEquip = false;
	bEquipped = true;

	State->SetIdleMode();
}

void UCEquipment::Unequip()
{
	bEquipped = false;

	if (OnUnequip.IsBound())
		OnUnequip.Broadcast();
}