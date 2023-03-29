#include "Weapon/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoActionComponent.h"
#include "GameFramework/Character.h"
#include "Trace/Detail/Field.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	DoActionClass = UCDoActionComponent::StaticClass();
}

TSubclassOf<ACAttachment> UCWeaponAsset::GetAttachmentClass()
{
	CheckNullResult(AttachmentClass,nullptr);
	return AttachmentClass;
}

void UCWeaponAsset::CallBeginPlay(ACharacter * InOwner)
{
	Equipment = NewObject<UCEquipment>(this);
	Equipment->BeginPlay(InOwner, EquipmentData);
	DoAction = NewObject<UCDoActionComponent>(this, DoActionClass);
	DoAction->BeginPlay( Equipment, InOwner, DoActionDatas, HitDatas);
}

//void UCWeaponAsset::GetAttachmentClass()
//{
//	//if (AttachmentClass != nullptr)
//	//{
//
//
//	//	Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
//	//}
//	//
//	//Equipment = NewObject<UCEquipment>(this);
//	//Equipment->BeginPlay(InOwner, EquipmentData);
//
//	//if (Attachment != nullptr)
//	//{
//	//	Equipment->OnBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
//	//	Equipment->OnUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
//	//}
//
//	//if (DoActionClass != nullptr)
//	//{
//	//	DoAction = NewObject<UCDoActionComponent>(this, DoActionClass);
//	//	DoAction->BeginPlay(Trace::Attachment, Equipment, InOwner, DoActionDatas, HitDatas);
//
//	//	if (Attachment != nullptr)
//	//	{
//	//		Attachment->OnAttachmentCollision.AddUFunction(DoAction, "OnAttachmentCollision");
//	//		Attachment->OffAttachmentCollision.AddUFunction(DoAction, "OffAttachmentCollision");
//
//	//		Attachment->OnAttachmentBeginOverlap.AddUFunction(DoAction, "OnAttachmentBeginOverlap");
//	//		Attachment->OnAttachmentEndOverlap.AddUFunction(DoAction, "OnAttachmentEndOverlap");
//	//	}
//	//}
//}