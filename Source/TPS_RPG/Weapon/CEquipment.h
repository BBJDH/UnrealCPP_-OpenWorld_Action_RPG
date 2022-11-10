#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/CWeaponStructures.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBehavior);

UCLASS()
class TPS_RPG_API UCEquipment : public UObject
{
	GENERATED_BODY()
	
public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

public:
	void Equip();
	void Begin_Equip();
	void End_Equip();
	void Unequip();

public:
	FEquipmentBehavior OnBeginEquip;
	FEquipmentBehavior OnUnequip;
	

private:
	class ACharacter* OwnerCharacter;

private:
	class UCMoveComponent* Move;
	class UCStateComponent* State;

private:
	FEquipmentData Data;

private:
	bool bBeginEquip;
	bool bEquipped;
};
