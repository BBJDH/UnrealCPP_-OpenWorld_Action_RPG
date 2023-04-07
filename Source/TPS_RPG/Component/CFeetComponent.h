
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"


USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector LeftDistance;	
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector RightDistance;	
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector PelvisDistance;	
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FRotator LeftRotation;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FRotator RightRotation;
};

/*=================================================
 *					UCFeetComponent
 *=================================================
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_RPG_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:	
	UCFeetComponent();

	//Feet IK On/Off
	UFUNCTION()
		void StartInAir();
	UFUNCTION()
		void EndInAir();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE const FFeetData& GetData() { return CurrentIKData; }

protected:
	virtual void BeginPlay() override;


private:
	void Trace(FName InName, float & OutDistance, FRotator& OutRotation) const ;

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float InterpolationSpeed = 50;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float DistanceOfDownFeet = 88;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float OffsetDistance = 0;

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName LeftSocketName = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName RightSocketName = "Foot_R";

	UPROPERTY(VisibleDefaultsOnly)
	class ACharacter* OwnerCharacter;

	FFeetData CurrentIKData;

	bool IsOnTrace;

		
};
