#include "Weapon/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	DoActionClass = UCDoAction::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner)
{
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}
	
	Equipment = NewObject<UCEquipment>(this);
	Equipment->BeginPlay(InOwner, EquipmentData);

	if (!!Attachment)
	{
		Equipment->OnBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
		Equipment->OnUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
	}

	if (!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas, HitDatas);

		if (!!Attachment)
		{
			Attachment->OnAttachmentCollision.AddUFunction(DoAction, "OnAttachmentCollision");
			Attachment->OffAttachmentCollision.AddUFunction(DoAction, "OffAttachmentCollision");

			Attachment->OnAttachmentBeginOverlap.AddUFunction(DoAction, "OnAttachmentBeginOverlap");
			Attachment->OnAttachmentEndOverlap.AddUFunction(DoAction, "OnAttachmentEndOverlap");
		}
	}
}