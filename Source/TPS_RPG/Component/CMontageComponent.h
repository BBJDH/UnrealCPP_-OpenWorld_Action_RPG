
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CMontageComponent.generated.h"


UENUM(BlueprintType)
enum class EMontageType : uint8
{
	Idle=0, Roll, Equip, Hitted, Dead, Action, Jump_First, Jump_Second, Heavy_Landed, Max,
};

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EMontageType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_RPG_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		class UDataTable* MontageData;

public:	
	UCMontageComponent();

protected:
	virtual void BeginPlay() override;


public:
	void PlayHitted()const;
	void PlayDead()const;
	void PlayFirstJump()const;
	void PlaySecondJump()const;
	void PlayLended()const;

private:
	void PlayAnimMontage(EMontageType const InType)const;

private:
	class ACharacter* Owner;

	FMontageData* Datas[static_cast<int32>(EMontageType::Max)];
		
};
