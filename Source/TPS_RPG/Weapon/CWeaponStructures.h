#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;

};

UENUM()
enum class EActionType
{
	Normal, Airborne, R_Skill, Execution, Ctrl_1_Skill, Max
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EActionType ActionCommand;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;


	UPROPERTY(EditAnywhere)
		bool FixedCamera;

	UPROPERTY(EditAnywhere)
		bool InAir;

public:
	void DoAction(class ACharacter* InOwner);
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float Launch = 100;

	UPROPERTY(EditAnywhere)
		float StopTime;

	UPROPERTY(EditAnywhere)
		class USoundBase* Sound;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		bool IsLaunchAttacker;

	UPROPERTY(EditAnywhere)
		FRotator LaunchRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void PlayMontage(class ACharacter* InOwner);
	void SendDamage(class ACharacter* InAttacker, class ACharacter* InOther);
	void PlayHitStop(class UWorld* InWorld);
	void PlaySound(class ACharacter* InOwner);
	void PlayEffect(class UWorld* InWorld, const FVector& InLocation);


};

USTRUCT()
struct FHitDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData;
};

UCLASS()
class TPS_RPG_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
