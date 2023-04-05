
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIStateComponent.generated.h"


UENUM(BlueprintType)
enum class EAIState : uint8
{
	Wait=0, Trace, Battle, Max,
};
DECLARE_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChaged, EAIState, EAIState)

UCLASS( ClassGroup=(Custom) )
class TPS_RPG_API UCAIStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAIStateComponent();

	bool IsWaitMode()const;
	bool IsTraceMode()const;
	bool IsBattleMode()const;

	void SetWaitMode()const;
	void SetTraceMode()const;
	void SetBattleMode()const;

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
protected:
	virtual void BeginPlay() override;

private:
	EAIState GetType()const;
	void ChangeType(EAIState const InType)const;

	//��� ����
public:
	//�ܺο��� ���ε�
	FAIStateTypeChaged OnAIStateTypeChaged;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UBlackboardComponent* Blackboard;

	//������ ��� �̸�
	UPROPERTY(EditAnywhere, Category = "Key")
		FName KeyNameOfAIState = "AIStateType";
};
