

#include "Component/CAINormalBehaviorComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Human/AI/CAIController.h"
#include "Global.h"

UCAINormalBehaviorComponent::UCAINormalBehaviorComponent()
{
}

bool UCAINormalBehaviorComponent::IsWaitMode() const
{
	return GetType() == EAINormalBehavior::Idle;
}

bool UCAINormalBehaviorComponent::IsStopAIMode() const
{
	return GetType() == EAINormalBehavior::StopAI;
}

bool UCAINormalBehaviorComponent::IsPatrolMode() const
{
	return GetType() == EAINormalBehavior::Patrol;
}

bool UCAINormalBehaviorComponent::IsBattleMode() const
{
	return GetType() == EAINormalBehavior::Battle;
}

void UCAINormalBehaviorComponent::SetIdleMode() const
{
	ChangeType(EAINormalBehavior::Idle);
}

void UCAINormalBehaviorComponent::SetStopAIMode() const
{
	ChangeType(EAINormalBehavior::StopAI);
}

void UCAINormalBehaviorComponent::SetPatrolMode() const
{
	ChangeType(EAINormalBehavior::Patrol);
}

void UCAINormalBehaviorComponent::SetBattleMode() const
{
	ChangeType(EAINormalBehavior::Battle);
}

void UCAINormalBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
	AAIController * OwnerController =  Cast<AAIController>(GetOwner());
	CheckNullUObject(OwnerController);
	SetBlackboard(OwnerController->GetBlackboardComponent());
}

EAINormalBehavior UCAINormalBehaviorComponent::GetType() const
{
	return static_cast<EAINormalBehavior>(Blackboard->GetValueAsEnum(KeyNameOfAIState));
}

void UCAINormalBehaviorComponent::ChangeType(EAINormalBehavior const InType) const
{

	EAINormalBehavior const PrevType = this->GetType();
	Blackboard->SetValueAsEnum(KeyNameOfAIState, static_cast<uint8>(InType));

	if (OnAIStateTypeChaged.IsBound())
		OnAIStateTypeChaged.Broadcast(PrevType,InType);
}

