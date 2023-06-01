#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class TPS_RPG_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	


public:
	//FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoActionComponent* GetDoAction() { return DoAction; }

	UCWeaponAsset();

	TSubclassOf<class ACAttachment> GetAttachmentClass();

	void CallBeginPlay(ACharacter* InOwner);



private:
	//UPROPERTY()
	//	class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoActionComponent* DoAction;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;


	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoActionComponent> DoActionClass;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;
};
