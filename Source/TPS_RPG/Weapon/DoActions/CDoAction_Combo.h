#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS(Blueprintable)
class TPS_RPG_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OffAttachmentCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class UShapeComponent* InCollision, class ACharacter* InOther) override;

private:
	int32 Index;

	bool bEnable;
	bool bExist;

private:
	TArray<class ACharacter*> Hitted;
};
