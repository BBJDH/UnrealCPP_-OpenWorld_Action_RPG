#include "Weapon/DoActions/CDoComboActionComponent.h"

#include "GenericTeamAgentInterface.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCDoComboActionComponent::DoAction()
{
	Super::DoAction();
	CHECK_TRUE(DoActionDatas.Num() < 0)

	if (IsComboEnable)
	{
		IsComboEnable = false;
		IsNextComboReady = true;

		return;
	}

	CHECK_TRUE(State->IsIdleMode()==false);
	CHECK_TRUE(ActionIndex > DoActionDatas.Num());

	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::DoUpperAction()
{
	Super::DoUpperAction();

	CHECK_TRUE(DoActionDatas.Num() < 0);
	CHECK_TRUE(State->IsIdleMode()==false);
	CHECK_TRUE(ActionIndex > DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::Do_R_Action()
{
	Super::Do_R_Action();

	CHECK_TRUE(DoActionDatas.Num() < 0);
	CHECK_TRUE(State->IsIdleMode()==false);
	CHECK_TRUE(ActionIndex > DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::Begin_DoAction()
{
	Super::Begin_DoAction();
	CHECK_TRUE(IsNextComboReady==false);
	
	IsNextComboReady = false;
	++ActionIndex;

	CHECK_TRUE(ActionIndex > DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::End_DoAction()
{
	Super::End_DoAction();
}

void UCDoComboActionComponent::OffAttachmentCollision()
{
	Super::OffAttachmentCollision();

	// if (DoActionDatas[ActionIndex].FixedCamera)
	// {
	// 	float Angle = -2.0f;
	// 	ACharacter* Candidate = nullptr;
	// 	for (ACharacter* Hitted : HittedCharacters)
	// 	{
	// 		FVector direction = Hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
	// 		direction = direction.GetSafeNormal2D();
	//
	// 		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
	//
	// 		float Dot = FVector::DotProduct(direction, forward);
	// 		if (Dot >= Angle)
	// 		{
	// 			Angle = Dot;
	// 			Candidate = Hitted;
	// 		}
	// 	}
	//
	// 	if (Candidate != nullptr)
	// 	{
	// 		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Candidate->GetActorLocation());
	// 		FRotator Target = FRotator(0, Rotator.Yaw, 0);
	//
	// 		AController* Controller = OwnerCharacter->GetController<AController>();
	// 		CHECK_NULL_UOBJECT(Controller);
	// 		Controller->SetControlRotation(Target);
	// 	}
	// }

	HittedCharacters.Empty();
}

void UCDoComboActionComponent::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCollision, InOther);
	CHECK_NULL_UOBJECT(InOther);
	CHECK_TRUE(ActionIndex > HitDatas.Num())
	
	IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
	CHECK_NULL(TeamAgentInterface);
	uint8 OwnerID = TeamAgentInterface->GetGenericTeamId();
	CHECK_TRUE(OwnerID == Cast<IGenericTeamAgentInterface>(InOther)->GetGenericTeamId());
	for (ACharacter const * elem : HittedCharacters)
	{
		// elem은 충돌체 안에 충돌한 Character 클래스 중복 체크
		// 이미 충돌했던 액터들과 일치한다면 여기서 탈출
		CHECK_TRUE(elem == InOther);
	}
	//새로 충돌된 객체들만 여기서 호출
	HittedCharacters.Add(InOther);
	HitDatas[ActionIndex].SendDamage(InAttacker, InOther);
}

