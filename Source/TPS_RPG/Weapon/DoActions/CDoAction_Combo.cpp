#include "Weapon/DoActions/CDoAction_Combo.h"

#include "GenericTeamAgentInterface.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
	Super::DoAction();
	CheckFalse(DoActionDatas.Num() > 0);

	if (IsComboEnable)
	{
		IsComboEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());
	CheckFalse(ActionIndex < DoActionDatas.Num());

	DoActionDatas[ActionIndex].DoAction(Owner);
}

void UCDoAction_Combo::DoUpperAction()
{
	Super::DoUpperAction();


	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());
	CheckFalse(ActionIndex < DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(Owner);
}

void UCDoAction_Combo::Do_R_Action()
{
	Super::Do_R_Action();

	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());
	CheckFalse(ActionIndex < DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(Owner);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	//TODO: 콤보 인덱스 재처리,Action Name으로 찾은 인덱스 + Index
	//Idle로 돌아온다면 Index 초기화,
	++ActionIndex;

	CheckFalse(ActionIndex < DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(Owner);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();
}

void UCDoAction_Combo::OffAttachmentCollision()
{
	Super::OffAttachmentCollision();

	if (DoActionDatas[ActionIndex].FixedCamera)
	{
		float angle = -2.0f;
		ACharacter* candidate = nullptr;
		for (ACharacter* hitted : HittedCharacters)
		{
			FVector direction = hitted->GetActorLocation() - Owner->GetActorLocation();
			direction = direction.GetSafeNormal2D();

			FVector forward = FQuat(Owner->GetControlRotation()).GetForwardVector();

			float dot = FVector::DotProduct(direction, forward);
			if (dot >= angle)
			{
				angle = dot;
				candidate = hitted;
			}
		}

		if (!!candidate)
		{
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), candidate->GetActorLocation());
			FRotator target = FRotator(0, rotator.Yaw, 0);

			AController* controller = Owner->GetController<AController>();
			controller->SetControlRotation(target);
		}
	}

	HittedCharacters.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCollision, InOther);
	CheckNull(InOther);
	CheckFalse(ActionIndex < HitDatas.Num());
	IGenericTeamAgentInterface* teamagentInterface = Cast<IGenericTeamAgentInterface>(Owner);
	CheckNull(teamagentInterface);
	uint8 ownerID = teamagentInterface->GetGenericTeamId();
	CheckTrue(ownerID == Cast<IGenericTeamAgentInterface>(InOther)->GetGenericTeamId());
	for (auto const & elem : HittedCharacters)
	{
		// elem은 충돌체 안에 충돌한 Character 클래스 중복 체크
		// 이미 충돌했던 액터들과 일치한다면 여기서 탈출
		CheckTrue(elem == InOther);
	}
	//새로 충돌된 객체들만 여기서 호출
	HittedCharacters.Add(InOther);
	HitDatas[ActionIndex].SendDamage(InAttacker, InOther);
}

