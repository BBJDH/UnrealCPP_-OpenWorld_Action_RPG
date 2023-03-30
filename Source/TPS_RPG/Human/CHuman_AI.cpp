

#include "Human/CHuman_AI.h"
#include "Global.h"
#include "Component/CAIStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_HealthBar.h"

ACHuman_AI::ACHuman_AI():ACHuman()
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

float ACHuman_AI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UCUserWidget_HealthBar* healthBar = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());

	if(healthBar != nullptr)
		healthBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

	return damage;
}

void ACHuman_AI::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidget->InitWidget();
	
	UCUserWidget_HealthBar * healthBar = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());
	CheckNull(healthBar);

	healthBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	healthBar->SetNameText(this->GetName());
	healthBar->SetControllerNameText(this->GetController()->GetName());

}
