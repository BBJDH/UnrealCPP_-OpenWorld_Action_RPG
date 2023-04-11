
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAINormalBehaviorComponent.generated.h"


UENUM(BlueprintType)
enum class EAINormalBehavior : uint8
{
	Idle=0,StopAI, Patrol, Battle, Max,
};
DECLARE_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChaged, EAINormalBehavior, EAINormalBehavior)

UCLASS( ClassGroup=(Custom) )
class TPS_RPG_API UCAINormalBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAINormalBehaviorComponent();

	bool IsWaitMode()const;
	bool IsStopAIMode()const;
	bool IsPatrolMode()const;
	bool IsBattleMode()const;

	void SetIdleMode()const;
	void SetStopAIMode()const;
	void SetPatrolMode()const;
	void SetBattleMode()const;

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
protected:
	virtual void BeginPlay() override;

private:
	EAINormalBehavior GetType()const;
	void ChangeType(EAINormalBehavior const InType)const;

	//멤버 변수
public:
	//외부에서 바인딩
	FAIStateTypeChaged OnAIStateTypeChaged;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UBlackboardComponent* Blackboard;

	//블랙보드 멤버 이름
	UPROPERTY(EditAnywhere, Category = "Key")
		FName KeyNameOfAIState = "NormalBehavior";
};
