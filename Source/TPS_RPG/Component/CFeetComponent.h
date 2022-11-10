
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"


USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

		/*************************************
		FVector LeftDistance;	//X 왼발과 땅의 간격
		FVector RightDistance;	//X 오른발과 빵의 간격
		FVector PelvisDistance;	//Z 허리 높낮이
		FVector LeftRotation;
		FVector RightRotation;
	**************************************/
	
public:
	//다시 새겨보자 펠비스는 최초 Z축이 위를 향하도록 되어있지만
	//아래로 내려가며 행렬 곱을 하면서 발에서는 X축이 위를 향하는 축으로 되어있다 기억하자
	
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector LeftDistance;	//X 왼발과 땅의 간격
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector RightDistance;	//X 오른발과 빵의 간격
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector PelvisDistance;	//Z 허리 높낮이

	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FRotator RightRotation;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_RPG_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:	
	UCFeetComponent();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	FORCEINLINE const FFeetData& GetData() { return Data; }


protected:
	virtual void BeginPlay() override;


private:
	void Trace(FName InName, float & OutDistance, FRotator& OutRotation);





private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;
	//

	UPROPERTY(EditAnywhere, Category = "Trace")
		float InterpSpeed = 50;
	//발을 붙이기까지의 시간

	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 88;
	//땅하고 발의 높이(추적/수정할 높이)




	UPROPERTY(EditAnywhere, Category = "Trace")
		FName LeftSocket = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName RightSocket = "Foot_R";

private:
	class ACharacter* OwnerCharacter;
	//컴포넌트 오너

	FFeetData Data;



		
};
