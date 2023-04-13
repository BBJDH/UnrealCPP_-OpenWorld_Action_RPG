#include "Weapon/DoActions/CDoComboActionComponent.h"

#include "GenericTeamAgentInterface.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCDoComboActionComponent::DoAction()
{
	Super::DoAction();
	CheckTrue(DoActionDatas.Num() < 0)

	if (IsComboEnable)
	{
		IsComboEnable = false;
		bExist = true;

		return;
	}

	CheckTrue(State->IsIdleMode()==false);
	CheckTrue(ActionIndex > DoActionDatas.Num());

	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::DoUpperAction()
{
	Super::DoUpperAction();

	CheckTrue(DoActionDatas.Num() < 0);
	CheckTrue(State->IsIdleMode()==false);
	CheckTrue(ActionIndex > DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::Do_R_Action()
{
	Super::Do_R_Action();

	CheckTrue(DoActionDatas.Num() < 0);
	CheckTrue(State->IsIdleMode()==false);
	CheckTrue(ActionIndex > DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckTrue(bExist==false);
	
	bExist = false;
	//TODO: �޺� �ε��� ��ó��,Action Name���� ã�� �ε��� + Index
	//Idle�� ���ƿ´ٸ� Index �ʱ�ȭ,
	++ActionIndex;

	CheckTrue(ActionIndex > DoActionDatas.Num());
	DoActionDatas[ActionIndex].DoAction(OwnerCharacter);
}

void UCDoComboActionComponent::End_DoAction()
{
	Super::End_DoAction();
}

void UCDoComboActionComponent::OffAttachmentCollision()
{
	Super::OffAttachmentCollision();

	if (DoActionDatas[ActionIndex].FixedCamera)
	{
		float angle = -2.0f;
		ACharacter* candidate = nullptr;
		for (ACharacter* hitted : HittedCharacters)
		{
			FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
			direction = direction.GetSafeNormal2D();

			FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

			float dot = FVector::DotProduct(direction, forward);
			if (dot >= angle)
			{
				angle = dot;
				candidate = hitted;
			}
		}

		if (candidate != nullptr)
		{
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
			FRotator target = FRotator(0, rotator.Yaw, 0);

			AController* controller = OwnerCharacter->GetController<AController>();
			controller->SetControlRotation(target);
		}
	}

	HittedCharacters.Empty();
}

void UCDoComboActionComponent::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCollision, InOther);
	CheckNullUObject(InOther);
	CheckTrue(ActionIndex > HitDatas.Num())
	
	IGenericTeamAgentInterface* teamagentInterface = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
	CheckNull(teamagentInterface);
	uint8 ownerID = teamagentInterface->GetGenericTeamId();
	CheckTrue(ownerID == Cast<IGenericTeamAgentInterface>(InOther)->GetGenericTeamId());
	for (auto const & elem : HittedCharacters)
	{
		// elem�� �浹ü �ȿ� �浹�� Character Ŭ���� �ߺ� üũ
		// �̹� �浹�ߴ� ���͵�� ��ġ�Ѵٸ� ���⼭ Ż��
		CheckTrue(elem == InOther);
	}
	//���� �浹�� ��ü�鸸 ���⼭ ȣ��
	HittedCharacters.Add(InOther);
	HitDatas[ActionIndex].SendDamage(InAttacker, InOther);
}

