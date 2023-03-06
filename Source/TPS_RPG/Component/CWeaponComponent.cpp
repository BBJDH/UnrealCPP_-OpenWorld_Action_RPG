#include "Component/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapon/CWeaponAsset.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoActionComponent.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	for (int i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (DataAssets[i]!= nullptr)
			DataAssets[i]->BeginPlay(Owner);
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckNullResult(DataAssets[(int32)Type], nullptr);

	return DataAssets[(int32)Type]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckNullResult(DataAssets[(int32)Type], nullptr);

	return DataAssets[(int32)Type]->GetEquipment();
}

UCDoActionComponent* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckNullResult(DataAssets[(int32)Type], nullptr);

	if (DataAssets[(int32)Type]->GetDoAction() ==nullptr)
	{
		return nullptr;
	}

	return DataAssets[(int32)Type]->GetDoAction();
}

void UCWeaponComponent::InitComboIndex()
{
	class UCDoActionComponent* CurrentDoActionComponent = GetDoAction();

	if (CurrentDoActionComponent == nullptr)
	{
		return;
	}

	CurrentDoActionComponent->InitIndex();
}

void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->Unequip();
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetSwordMode()
{
	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetGreatSwordMode()
{
	SetMode(EWeaponType::GreatSword);
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if (DataAssets[(int32)InType] != nullptr)
	{
		DataAssets[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType PrevType = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(PrevType, InType);
}

void UCWeaponComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (GetDoAction()!=nullptr)
		GetDoAction()->DoAction();
}

void UCWeaponComponent::DoUpperAction()
{
	CheckTrue(IsUnarmedMode());
	if (GetDoAction() != nullptr)
		GetDoAction()->DoUpperAction();
}

void UCWeaponComponent::Do_R_Action()
{
	CheckTrue(IsUnarmedMode());
	if (GetDoAction() != nullptr)
		GetDoAction()->Do_R_Action();
}
