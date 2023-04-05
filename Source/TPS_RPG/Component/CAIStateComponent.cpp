

#include "Component/CAIStateComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UCAIStateComponent::UCAIStateComponent()
{
}

bool UCAIStateComponent::IsWaitMode() const
{
	return GetType() == EAIState::Wait;
}

bool UCAIStateComponent::IsTraceMode() const
{
	return GetType() == EAIState::Trace;
}

bool UCAIStateComponent::IsBattleMode() const
{
	return GetType() == EAIState::Battle;
}

void UCAIStateComponent::SetWaitMode() const
{
	ChangeType(EAIState::Wait);
}

void UCAIStateComponent::SetTraceMode() const
{
	ChangeType(EAIState::Trace);
}

void UCAIStateComponent::SetBattleMode() const
{
	ChangeType(EAIState::Battle);
}

void UCAIStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

EAIState UCAIStateComponent::GetType() const
{
	return static_cast<EAIState>(Blackboard->GetValueAsEnum(KeyNameOfAIState));
}

void UCAIStateComponent::ChangeType(EAIState const InType) const
{

	EAIState const PrevType = this->GetType();
	Blackboard->SetValueAsEnum(KeyNameOfAIState, static_cast<uint8>(InType));

	if (OnAIStateTypeChaged.IsBound())
		OnAIStateTypeChaged.Broadcast(PrevType,InType);
}

