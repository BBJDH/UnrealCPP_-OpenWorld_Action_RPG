
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
		FVector LeftDistance;	//X �޹߰� ���� ����
		FVector RightDistance;	//X �����߰� ���� ����
		FVector PelvisDistance;	//Z �㸮 ������
		FVector LeftRotation;
		FVector RightRotation;
	**************************************/
	
public:
	//�ٽ� ���ܺ��� ��񽺴� ���� Z���� ���� ���ϵ��� �Ǿ�������
	//�Ʒ��� �������� ��� ���� �ϸ鼭 �߿����� X���� ���� ���ϴ� ������ �Ǿ��ִ� �������
	
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector LeftDistance;	//X �޹߰� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector RightDistance;	//X �����߰� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector PelvisDistance;	//Z �㸮 ������

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
	//���� ���̱������ �ð�

	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 88;
	//���ϰ� ���� ����(����/������ ����)




	UPROPERTY(EditAnywhere, Category = "Trace")
		FName LeftSocket = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName RightSocket = "Foot_R";

private:
	class ACharacter* OwnerCharacter;
	//������Ʈ ����

	FFeetData Data;



		
};
