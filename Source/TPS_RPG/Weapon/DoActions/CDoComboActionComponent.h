#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoActionComponent.h"
#include "CDoComboActionComponent.generated.h"

UCLASS(Blueprintable)
class TPS_RPG_API UCDoComboActionComponent : public UCDoActionComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { IsComboEnable = true; }
	FORCEINLINE void DisableCombo() { IsComboEnable = false; }

public:
	virtual void DoAction() override;
	virtual void DoUpperAction() override;
	virtual void Do_R_Action() override;


	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OffAttachmentCollision() override;
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther) override;

private:
	bool IsComboEnable;
	bool IsNextComboReady;

private:
	TArray<class ACharacter*> HittedCharacters;
};
