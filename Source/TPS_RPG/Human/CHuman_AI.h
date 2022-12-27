
#pragma once

#include "CoreMinimal.h"
#include "Human/CHuman.h"
#include "CHuman_AI.generated.h"


UCLASS()
class TPS_RPG_API ACHuman_AI : public ACHuman
{
	GENERATED_BODY()

public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	ACHuman_AI();

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2;

protected:
	virtual void BeginPlay() override;

};
