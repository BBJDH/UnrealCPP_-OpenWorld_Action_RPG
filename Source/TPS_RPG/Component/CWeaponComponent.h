#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist, Sword, Hammer, Warp, Rotator, Bow, Dual, GreatSword, Max, 
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);


UCLASS( ClassGroup=(Custom) )
class TPS_RPG_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCWeaponComponent();



	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoActionComponent* GetDoAction();

	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsSwordMode() { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode() { return Type == EWeaponType::Hammer; }
	FORCEINLINE bool IsGreatSwordMode() { return Type == EWeaponType::GreatSword; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitComboIndex();
	void SetUnarmedMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetGreatSwordMode();

	void DoAction();
	void DoUpperAction();
	void Do_R_Action();



protected:
	virtual void BeginPlay() override;

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:

	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

	class ACharacter* Owner;
	EWeaponType Type = EWeaponType::Max;

};
