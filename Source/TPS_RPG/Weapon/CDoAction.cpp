#include "Weapon/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"

UCDoAction::UCDoAction()
{

}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Owner = InOwner;
	World = Owner->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Status = CHelpers::GetComponent<UCStatusComponent>(Owner);
	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoAction::Tick(float InDeltaTime)
{

}

void UCDoAction::DoAction()
{
	ChangedType(EActionType::Normal);
}

void UCDoAction::DoUpperAction()
{
	ChangedType(EActionType::Airborne);
}

void UCDoAction::Begin_DoAction()
{
	BeginAction = true;
}

void UCDoAction::End_DoAction()
{
	BeginAction = false;

	State->SetIdleMode();
	ChangedType(EActionType::Normal);
}
void UCDoAction::FindActionIdex(EActionType const NewType)
{
	//FDoActionData ���� status�� Inair, ���� ������ �׼� Ŀ��带 Ȯ��
	ActionIndex = 0;
	for (auto const& elem : DoActionDatas)
	{
		if (Status->IsInAir() == elem.InAir and elem.ActionCommand == NewType)
			break;
		ActionIndex++;
	}
}

void UCDoAction::ChangedType(EActionType const NewType)
{
	EActionType const PrevType = this->ActionType;
	this->ActionType = NewType;

	if(PrevType != NewType)
		FindActionIdex(NewType);
	//�ܺ� �̺�Ʈ�� ���� ���ε� ó���صξ�� ��  ChangedType �̿�
	//if (OnActionTypeChaged.IsBound())
	//	OnActionTypeChaged.Broadcast(PrevType, NewType);
}
