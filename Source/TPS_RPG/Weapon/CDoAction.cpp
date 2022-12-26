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
	CLog::Print(Status->IsInAir());
}

void UCDoAction::DoAction()
{
	ChangedType(EActionType::Normal);
}

void UCDoAction::DoUpperAction()
{
	ChangedType(EActionType::Airborne);
}

void UCDoAction::Do_R_Action()
{
	ChangedType(EActionType::R_Skill);
}

void UCDoAction::Begin_DoAction()
{
	BeginAction = true;
}

void UCDoAction::End_DoAction()
{
	BeginAction = false;
	State->SetIdleMode();
	FindActionIdex(ActionType);
}

void UCDoAction::FindActionIdex(EActionType const NewType)
{
	//FDoActionData 에서 status의 Inair, 현재 무기의 액션 커멘드를 확인

	for(int i=0; i< DoActionDatas.Max(); i++)
	{
		if(Status->IsInAir() == DoActionDatas[i].InAir and DoActionDatas[i].ActionCommand == NewType)
		{
			ActionIndex = i;
			break;
		}
	}
}

void UCDoAction::ChangedType(EActionType const NewType)
{
	EActionType const PrevType = this->ActionType;
	this->ActionType = NewType;
	if(PrevType != NewType)
		FindActionIdex(NewType);
	//외부 이벤트로 사용시 바인딩 처리해두어야 함  ChangedType 이용
	//if (OnActionTypeChaged.IsBound())
	//	OnActionTypeChaged.Broadcast(PrevType, NewType);
}
