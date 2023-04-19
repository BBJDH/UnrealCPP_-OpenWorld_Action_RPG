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

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	for (int i = 0; i < static_cast<int32>(EWeaponType::Max); i++)
	{
		if (DataAssets[i]!= nullptr)
		{
			FActorSpawnParameters params;
			params.Owner = OwnerCharacter;
			TSubclassOf<ACAttachment> AttachmentClass = DataAssets[i]->GetAttachmentClass();
			if(IsValid(AttachmentClass)==true)
			{
				Attachments[i]= OwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
				DataAssets[i]->CallBeginPlay(OwnerCharacter);
			}
			if (Attachments[i] != nullptr AND DataAssets[i]->GetDoAction() != nullptr)
			{
				Attachments[i]->OnAttachmentCollision.AddUFunction(DataAssets[i]->GetDoAction(), "OnAttachmentCollision");
				Attachments[i]->OffAttachmentCollision.AddUFunction(DataAssets[i]->GetDoAction(), "OffAttachmentCollision");

				Attachments[i]->OnAttachmentBeginOverlap.AddUFunction(DataAssets[i]->GetDoAction(), "OnAttachmentBeginOverlap");
				Attachments[i]->OnAttachmentEndOverlap.AddUFunction(DataAssets[i]->GetDoAction(), "OnAttachmentEndOverlap");
			}
		}
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CHECK_TRUE_RESULT(IsUnarmedMode(), nullptr);
	CHECK_NULL_RESULT(Attachments[static_cast<int32>(CurrentWeaponType)], nullptr);

	return Attachments[static_cast<int32>(CurrentWeaponType)];
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CHECK_TRUE_RESULT(IsUnarmedMode(), nullptr);
	CHECK_NULL_RESULT(DataAssets[static_cast<int32>(CurrentWeaponType)], nullptr);

	return DataAssets[static_cast<int32>(CurrentWeaponType)]->GetEquipment();
}

UCDoActionComponent* UCWeaponComponent::GetDoAction()
{
	CHECK_TRUE_RESULT(IsUnarmedMode(), nullptr);
	CHECK_NULL_RESULT(DataAssets[static_cast<int32>(CurrentWeaponType)], nullptr);

	if (DataAssets[static_cast<int32>(CurrentWeaponType)]->GetDoAction() ==nullptr)
	{
		return nullptr;
	}

	return DataAssets[static_cast<int32>(CurrentWeaponType)]->GetDoAction();
}

void UCWeaponComponent::NotifyBeginEquip()
{
	GetEquipment()->Begin_Equip(Attachments[static_cast<int32>(CurrentWeaponType)]);
}

void UCWeaponComponent::NotifyEndEquip()
{
	GetEquipment()->End_Equip();
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
	GetEquipment()->Unequip(Attachments[static_cast<int32>(CurrentWeaponType)]);
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

void UCWeaponComponent::SetMode(EWeaponType const InType)
{
	if (CurrentWeaponType == InType)
	{
		SetUnarmedMode();

		return;
	}

	if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip(Attachments[static_cast<int32>(InType)]);
	}

	if (DataAssets[static_cast<int32>(InType)] != nullptr)
	{
		DataAssets[static_cast<int32>(InType)]->GetEquipment()->Equip(Attachments[static_cast<int32>(InType)]);

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType const InType)
{
	EWeaponType const PrevType = CurrentWeaponType;
	CurrentWeaponType = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(PrevType, InType);
}

void UCWeaponComponent::DoAction()
{
	CHECK_TRUE(IsUnarmedMode());

	if (GetDoAction()!=nullptr)
		GetDoAction()->DoAction();
}

void UCWeaponComponent::DoUpperAction()
{
	CHECK_TRUE(IsUnarmedMode());
	if (GetDoAction() != nullptr)
		GetDoAction()->DoUpperAction();
}

void UCWeaponComponent::Do_R_Action()
{
	CHECK_TRUE(IsUnarmedMode());
	if (GetDoAction() != nullptr)
		GetDoAction()->Do_R_Action();
}

void UCWeaponComponent::DestroyWeapons()
{
	for(ACAttachment* elem : Attachments)
	{
		if(IsValid(elem)==true)
		{
			elem->Destroy();
		}
	}
}
