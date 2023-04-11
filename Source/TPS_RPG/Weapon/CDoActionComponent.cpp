#include "Weapon/CDoActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"



UCDoActionComponent::UCDoActionComponent()
{

}

void UCDoActionComponent::BeginPlay(/*ACAttachment* InAttachment, */UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	Status = Cast<UCStatusComponent>(OwnerCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoActionComponent::Tick(float InDeltaTime)
{
	CLog::Print(Status->IsInAir());
}

void UCDoActionComponent::DoAction()
{
	ChangedType(EActionType::Normal);
}

void UCDoActionComponent::DoUpperAction()
{
	ChangedType(EActionType::Airborne);
}

void UCDoActionComponent::Do_R_Action()
{
	ChangedType(EActionType::R_Skill);
}

void UCDoActionComponent::Begin_DoAction()
{
	BeginAction = true;
}

void UCDoActionComponent::End_DoAction()
{
	BeginAction = false;
	State->SetIdleMode();
	FindActionIdex(ActionType);
}

void UCDoActionComponent::InitIndex()
{
	FindActionIdex(EActionType::Normal);
}

void UCDoActionComponent::FindActionIdex(EActionType const NewType)
{
	
	if (DoActionDatas.IsValidIndex(0)==false or DoActionDatas.Max()==0 )
		return;

	for(int i=0; i< DoActionDatas.Max(); i++)
	{
		//if(OwnerCharacter == DoActionDatas[i].InAir and DoActionDatas[i].ActionCommand == NewType)
		if(Status->IsInAir() == DoActionDatas[i].InAir and DoActionDatas[i].ActionCommand == NewType)
		{
			ActionIndex = i;
			break;
		}
	}

	UE_LOG(GameProject, Display, TEXT("%d"), ActionIndex);

}

void UCDoActionComponent::ChangedType(EActionType const NewType)
{
	EActionType const PrevType = this->ActionType;
	this->ActionType = NewType;
	//액션이 들어올때 그 몽타주 인덱스를 찾도록 설정
	if(PrevType != NewType)
		FindActionIdex(NewType);
	//외부 이벤트로 사용시 바인딩 처리해두어야 함  ChangedType 이용
	//if (OnActionTypeChaged.IsBound())
	//	OnActionTypeChaged.Broadcast(PrevType, NewType);
}
