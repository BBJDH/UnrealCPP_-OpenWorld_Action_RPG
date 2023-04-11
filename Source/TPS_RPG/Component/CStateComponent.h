#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Roll, BackStep, Equip, GetHit, Dead, Action, Max, 
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);


UCLASS( ClassGroup=(Custom) )
class TPS_RPG_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode()		const { return Type == EStateType::Idle; }
	FORCEINLINE bool IsRollMode()		const { return Type == EStateType::Roll; }
	FORCEINLINE bool IsBackstepMode()	const { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsEquipMode()		const { return Type == EStateType::Equip; }
	FORCEINLINE bool IsGetHitMode()		const { return Type == EStateType::GetHit; }
	FORCEINLINE bool IsDeadMode()		const { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() 	const { return Type == EStateType::Action; }
	FORCEINLINE bool IsSubActionMode()	const { return bInSubActionMode; }

public:	
	UCStateComponent();

public:
	void SetIdleMode();
	void SetRollMode();
	void SetBackStepMode();
	void SetEquipMode();
	void SetGetHitMode();
	void SetDeadMode();
	void SetActionMode();

	void OnSubActionMode();
	void OffSubActionMode();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EStateType const InType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
	bool bInSubActionMode;
};
