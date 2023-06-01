#include "Component/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapon/CWeaponAsset.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoActionComponent.h"
#include "Utilities/CHelpers.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;



}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CHECK_NULL_UOBJECT(OwnerCharacter);

	//Data Asset으로부터 등록된 무기 생성
	for (int i = 0; i < static_cast<int32>(EWeaponType::Max); i++)
	{
		if (DataAssets[i] == nullptr)
		{
			continue;
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		//UE_LOG(GameProject,Warning, TEXT("SpawnActor Call!"));
		TSubclassOf<ACAttachment> AttachmentClass = DataAssets[i]->GetAttachmentClass();
		if(IsValid(AttachmentClass)==true)
		{
			Attachments[i]= OwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, SpawnParams);
			DataAssets[i]->CallBeginPlay(OwnerCharacter);
		}
		if (IsValid(Attachments[i]) == true AND IsValid(DataAssets[i]->GetDoAction()) == true)
		{
			//Attachments[i]->OnAttachmentCollision.AddUFunction(DataAssets[i]->GetDoAction(), "OnAttachmentCollision");
			//충돌한 캐릭터들 초기화를 위해 사용
			Attachments[i]->OffAttachmentCollision.AddUFunction(DataAssets[i]->GetDoAction(), "OffAttachmentCollision");

			//충돌한 새로운 개체 hit 처리
			Attachments[i]->OnAttachmentBeginOverlap.AddUFunction(DataAssets[i]->GetDoAction(), "OnAttachmentBeginOverlap");
			//Attachments[i]->OnAttachmentEndOverlap.AddUFunction(DataAssets[i]->GetDoAction(), "OnAttachmentEndOverlap");
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
	CHECK_NULL_UOBJECT_RESULT(Attachments[static_cast<int32>(CurrentWeaponType)], nullptr);

	return Attachments[static_cast<int32>(CurrentWeaponType)];
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CHECK_TRUE_RESULT(IsUnarmedMode(), nullptr);
	CHECK_NULL_UOBJECT_RESULT(DataAssets[static_cast<int32>(CurrentWeaponType)], nullptr);

	return DataAssets[static_cast<int32>(CurrentWeaponType)]->GetEquipment();
}

UCDoActionComponent* UCWeaponComponent::GetDoAction()
{
	CHECK_TRUE_RESULT(IsUnarmedMode(), nullptr);
	CHECK_NULL_UOBJECT_RESULT(DataAssets[static_cast<int32>(CurrentWeaponType)], nullptr);
	CHECK_NULL_UOBJECT_RESULT(DataAssets[static_cast<int32>(CurrentWeaponType)]->GetDoAction(),nullptr);

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
	CHECK_TRUE(IsUnarmedMode()==true);
	class UCDoActionComponent* CurrentDoActionComponent = GetDoAction();
	CHECK_NULL_UOBJECT(CurrentDoActionComponent);

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

	CHECK_NULL_UOBJECT(DataAssets[static_cast<int32>(InType)]);
	
	DataAssets[static_cast<int32>(InType)]->GetEquipment()->Equip(Attachments[static_cast<int32>(InType)]);

	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType const InType)
{
	EWeaponType const PrevType = CurrentWeaponType;
	CurrentWeaponType = InType;

	if (OnWeaponTypeChange.IsBound())
	{
		OnWeaponTypeChange.Broadcast(PrevType, InType);
	}
}

void UCWeaponComponent::DoAction()
{
	CHECK_TRUE(IsUnarmedMode());
	CHECK_NULL_UOBJECT(GetDoAction())

	GetDoAction()->DoAction();
}

void UCWeaponComponent::DoUpperAction()
{
	CHECK_TRUE(IsUnarmedMode());
	CHECK_NULL_UOBJECT(GetDoAction())

	GetDoAction()->DoUpperAction();
}

void UCWeaponComponent::Do_R_Action()
{
	CHECK_TRUE(IsUnarmedMode());
	CHECK_NULL_UOBJECT(GetDoAction())

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
