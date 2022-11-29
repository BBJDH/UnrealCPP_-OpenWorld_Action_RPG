#include "Weapon/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"

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

	ActionIndex = 0;
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

	for (auto const & elem : HittedCharacters)
		CheckTrue(elem == InOther);

	HittedCharacters.Add(InOther);
	HitDatas[ActionIndex].SendDamage(InAttacker, InOther);
}

