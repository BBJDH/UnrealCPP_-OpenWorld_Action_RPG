

#include "Component/CAIStateComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UCAIStateComponent::UCAIStateComponent()
{
}

bool UCAIStateComponent::IsWaitMode()
{
	return GetType() == EAIState::Wait;
}

bool UCAIStateComponent::IsTraceMode()
{
	return GetType() == EAIState::Trace;
}

bool UCAIStateComponent::IsBattleMode()
{
	return GetType() == EAIState::Battle;
}

void UCAIStateComponent::SetWaitMode()
{
	ChangeType(EAIState::Wait);
}

void UCAIStateComponent::SetTraceMode()
{
	ChangeType(EAIState::Trace);
}

void UCAIStateComponent::SetBattleMode()
{
	ChangeType(EAIState::Battle);
}

void UCAIStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

EAIState UCAIStateComponent::GetType() const
{
	return static_cast<EAIState>(Blackboard->GetValueAsEnum(AIStateKey));
}

void UCAIStateComponent::ChangeType(EAIState const InType) const
{

	EAIState const PrevType = this->GetType();
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<uint8>(InType));

	if (OnAIStateTypeChaged.IsBound())
		OnAIStateTypeChaged.Broadcast(PrevType,InType);
}

