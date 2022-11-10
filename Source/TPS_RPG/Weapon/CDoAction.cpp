#include "Weapon/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

UCDoAction::UCDoAction()
{

}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Owner = InOwner;
	World = Owner->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(Owner);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoAction::Tick(float InDeltaTime)
{

}

void UCDoAction::DoAction()
{

}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;

	State->SetIdleMode();

}