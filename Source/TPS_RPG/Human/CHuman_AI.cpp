

#include "Human/CHuman_AI.h"
#include "Global.h"
#include "Component/CAIStateComponent.h"

ACHuman_AI::ACHuman_AI()
{
	CHelpers::CreateActorComponent<UCAIStateComponent>(this, &AIState, "AIState");
}

void ACHuman_AI::BeginPlay()
{
	Super::BeginPlay();
}
