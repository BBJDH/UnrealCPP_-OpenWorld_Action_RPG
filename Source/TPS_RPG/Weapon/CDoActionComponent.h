#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/CWeaponStructures.h"
#include "CDoActionComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, EActionType);

UCLASS(Abstract)
class TPS_RPG_API UCDoActionComponent : public UObject
{
	GENERATED_BODY()
	
public:
	UCDoActionComponent();

	virtual void BeginPlay(class ACAttachment* InAttachment, class UCEquipment* InEquipment, class ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas);
	virtual void Tick(float InDeltaTime);

	UFUNCTION()
		virtual void OnAttachmentCollision() { }

	UFUNCTION()
		virtual void OffAttachmentCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther) { }
	
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther) { }

	virtual void DoAction();
	virtual void DoUpperAction();
	virtual void Do_R_Action();

	virtual void Begin_DoAction();
	virtual void End_DoAction();

	void InitIndex();

private:
	void FindActionIdex(EActionType const NewType);
	void ChangedType(EActionType const InType);
public:
	//FActionTypeChanged OnActionTypeChaged;

protected:
	class ACharacter* Owner;
	class UWorld* World;

	class UCStateComponent* State;
	class UCStatusComponent* Status;

	EActionType ActionType;
protected:
	int32 ActionIndex;
	bool BeginAction;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;
};
