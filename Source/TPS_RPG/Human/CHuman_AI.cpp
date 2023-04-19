

#include "Human/CHuman_AI.h"
#include "Global.h"
#include "Component/CAINormalBehaviorComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_HealthBar.h"

ACHuman_AI::ACHuman_AI():ACHuman()
{
	CHECK_NULL_UOBJECT(this->GetMesh());

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	CHECK_NULL_UOBJECT(HealthBarWidget);

	HealthBarWidget->SetupAttachment(this->GetMesh());

	ConstructorHelpers::FClassFinder<UCUserWidget_HealthBar> AssetFound(*FString("WidgetBlueprint'/Game/Widgets/WB_HealthBar.WB_HealthBar_C'"));
	CHECK_NULL_UOBJECT(AssetFound.Class);

	HealthBarWidget->SetWidgetClass(AssetFound.Class);
	HealthBarWidget->SetRelativeLocation(FVector(0, 0, 220));
	HealthBarWidget->SetDrawSize(FVector2D(120,20));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
}

float ACHuman_AI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float const Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CHECK_NULL_UOBJECT_RESULT(Status, Damage);

	UCUserWidget_HealthBar* HealthBarToUpdate = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());
	CHECK_NULL_UOBJECT_RESULT(HealthBarToUpdate, Damage);

	HealthBarToUpdate->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

	return Damage;
}

void ACHuman_AI::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidget->InitWidget();
	
	UCUserWidget_HealthBar * HealthBarToInit = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());
	CHECK_NULL_UOBJECT(HealthBarToInit);

	HealthBarToInit->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	HealthBarToInit->SetNameText(this->GetName());
	HealthBarToInit->SetControllerNameText(this->GetController()->GetName());
}
