

#include "Human/CHuman_AI.h"
#include "Global.h"
#include "Component/CAIStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_HealthBar.h"

ACHuman_AI::ACHuman_AI():ACHuman()
{
	CheckNullUObject(this->GetMesh());
	AIState = this->CreateDefaultSubobject<UCAIStateComponent>("AIState");

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	CheckNullUObject(HealthBarWidget);

	HealthBarWidget->SetupAttachment(this->GetMesh());

	ConstructorHelpers::FClassFinder<UCUserWidget_HealthBar> AssetFound(*FString("WidgetBlueprint'/Game/Widgets/WB_HealthBar.WB_HealthBar_C'"));
	CheckNullUObject(AssetFound.Class);

	HealthBarWidget->SetWidgetClass(AssetFound.Class);
	HealthBarWidget->SetRelativeLocation(FVector(0, 0, 220));
	HealthBarWidget->SetDrawSize(FVector2D(120,20));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
}

float ACHuman_AI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float const Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CheckNullUObjectResult(Status, Damage);

	UCUserWidget_HealthBar* HealthBarToUpdate = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());
	CheckNullUObjectResult(HealthBarToUpdate, Damage);

	HealthBarToUpdate->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

	return Damage;
}

void ACHuman_AI::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidget->InitWidget();
	
	UCUserWidget_HealthBar * HealthBarToInit = Cast<UCUserWidget_HealthBar>(HealthBarWidget->GetUserWidgetObject());
	CheckNullUObject(HealthBarToInit);

	HealthBarToInit->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	HealthBarToInit->SetNameText(this->GetName());
	HealthBarToInit->SetControllerNameText(this->GetController()->GetName());
}
