
#pragma once

#include "CoreMinimal.h"
#include "Human/CHuman.h"
#include "CHuman_AI.generated.h"


UCLASS()
class TPS_RPG_API ACHuman_AI : public ACHuman
{
	GENERATED_BODY()

public:

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	ACHuman_AI();

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;



protected:
	virtual void BeginPlay() override;

};
