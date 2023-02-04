
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

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthBarWidget;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIStateComponent* AIState;



};
