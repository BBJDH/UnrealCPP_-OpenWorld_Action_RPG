
#pragma once

#include "CoreMinimal.h"
#include "Human/CHuman.h"
#include "CHuman_AI.generated.h"


UCLASS()
class TPS_RPG_API ACHuman_AI : public ACHuman
{
	GENERATED_BODY()

public:
	ACHuman_AI();
	virtual void BeginPlay() override;

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthBarWidget;

//private:
//	UPROPERTY(VisibleDefaultsOnly)
//		class UCAINormalBehaviorComponent* AIState;



};
