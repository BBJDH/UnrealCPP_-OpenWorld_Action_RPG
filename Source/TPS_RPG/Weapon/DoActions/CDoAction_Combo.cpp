#include "Weapon/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
	Super::DoAction();
	CheckFalse(DoActionDatas.Num() > 0);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());
	CheckFalse(Index < DoActionDatas.Num());

	DoActionDatas[Index].DoAction(Owner);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;

	++Index;

	CheckFalse(Index < DoActionDatas.Num());
	DoActionDatas[Index].DoAction(Owner);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

void UCDoAction_Combo::OffAttachmentCollision()
{
	Super::OffAttachmentCollision();

	if (DoActionDatas[Index].bFixedCamera)
	{
		float angle = -2.0f;
		ACharacter* candidate = nullptr;
		for (ACharacter* hitted : Hitted)
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

	Hitted.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCollision, InOther);
	CheckNull(InOther);
	CheckFalse(Index < HitDatas.Num());

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.Add(InOther);
	HitDatas[Index].SendDamage(InAttacker, InOther);
}