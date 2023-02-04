

#include "Human/CHuman_AI.h"
#include "Global.h"
#include "Component/CAIStateComponent.h"
#include "Widgets/CUserWidget_HealthBar.h"

ACHuman_AI::ACHuman_AI()
{
	CHelpers::CreateActorComponent<UCAIStateComponent>(this, &AIState, "AIState");

	TSubclassOf<UCUserWidget_HealthBar> HealthBarClass;

	CHelpers::GetClass<UCUserWidget_HealthBar>(&HealthBarClass, "");
}

void ACHuman_AI::BeginPlay()
{
	Super::BeginPlay();
}
