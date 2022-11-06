
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGreatSword.generated.h"

UCLASS()
class TPS_RPG_API ACGreatSword : public AActor
{
	GENERATED_BODY()


	
public:	
	ACGreatSword();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void Equip();
	void Begin_Equip();
	void End_Equip();

	void UnEquip();
	void Begin_UnEquip();
	void End_UnEquip();


private:
	UPROPERTY(VisibleDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		class UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		class UAnimMontage* UnequipMontage;


private:
	class ACHuman* InOwner;


};
