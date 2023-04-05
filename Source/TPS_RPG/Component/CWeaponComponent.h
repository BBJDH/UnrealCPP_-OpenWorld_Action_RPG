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


	//Check Current Weapon Mode
	FORCEINLINE bool IsUnarmedMode() const { return CurrentWeaponType == EWeaponType::Max; }
	FORCEINLINE bool IsSwordMode() const { return CurrentWeaponType == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode() const { return CurrentWeaponType == EWeaponType::Hammer; }
	FORCEINLINE bool IsGreatSwordMode() const { return CurrentWeaponType == EWeaponType::GreatSword; }

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void InitComboIndex();

	//Set Current Weapon Mode
	void SetUnarmedMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetGreatSwordMode();

	//Owners Call, Weapon's Action Command
	void DoAction();
	void DoUpperAction();
	void Do_R_Action();

	//Destroy All Having Weapons
	void DestroyWeapons();

	//Notify Call
	void NotifyBeginEquip();
	void NotifyEndEquip();


protected:
	virtual void BeginPlay() override;

private:
	//Weapon Mode Control
	void SetMode(EWeaponType const InType);

	//Change Current Weapon Type
	void ChangeType(EWeaponType const InType);

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:

	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[static_cast<int32>(EWeaponType::Max)];

	UPROPERTY()
		class ACAttachment* Attachments[static_cast<int32>(EWeaponType::Max)];

	UPROPERTY(VisibleDefaultsOnly)
	class ACharacter* OwnerCharacter;
	EWeaponType CurrentWeaponType = EWeaponType::Max;

};
