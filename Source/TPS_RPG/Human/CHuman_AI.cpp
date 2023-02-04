

#include "Human/CHuman_AI.h"
#include "Global.h"
#include "Component/CAIStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_HealthBar.h"

ACHuman_AI::ACHuman_AI()
{
	CHelpers::CreateActorComponent<UCAIStateComponent>(this, &AIState, "AIState");
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthBarWidget, "HealthBar",this->GetMesh());

	TSubclassOf<UCUserWidget_HealthBar> healthBarClass;

	CHelpers::GetClass<UCUserWidget_HealthBar>(&healthBarClass, "WidgetBlueprint'/Game/Widgets/WB_HealthBar.WB_HealthBar_C'");

	HealthBarWidget->SetWidgetClass(healthBarClass);
	HealthBarWidget->SetRelativeLocation(FVector(0, 0, 220));
	HealthBarWidget->SetDrawSize(FVector2D(120,20));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
}

void ACHuman_AI::BeginPlay()
{
	Super::BeginPlay();


	//무조건 위젯 쓸때 BeginPlay에서 init 해줘야함
	HealthBarWidget->InitWidget();
	
	UCUserWidget_HealthBar * healthBar = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());
	healthBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	healthBar->SetNameText(this->GetName());
	healthBar->SetControllerNameText(this->GetController()->GetName());

}
