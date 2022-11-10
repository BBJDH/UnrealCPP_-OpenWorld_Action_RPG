

#include "Component/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

//#define LOG_UCFeetComponent 1

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DrawDebug = EDrawDebugTrace::Type::ForOneFrame;
}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance, rightDistance;		//�޾ƿ� �� ����
	FRotator leftRotation, rightRotation;	//�޾ƿ� �� ȸ����

	Trace(LeftSocket, leftDistance, leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);
	//����߿� ���� ���� ����(����)�� ������

	/*************************************
	Data �ȿ��� ���� �߰� ���� ����, ���� �����̼� ���� ����ִ�
	(�� ���Ʒ��� �󸶳� �������� ��������� �����Ѵ�)
	Pelvis�� ��������

	Offset�� ���� ���� ���� ���̸� ������ (-TraceDistance ~ +TraceDistance) 
	Pelvis�� ���� ������ ���̸� ���
**************************************/
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
	//��������(��Ÿ��) ������, ��ǥ��, ��Ÿ�ð�, �����ӵ�

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	//�Ѵ� ��Pelvis ��ŭ ���̸� �ø��ų� ����

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
#if LOG_UCFeetComponent
	CLog::Print(Data.PelvisDistance, 11);
	CLog::Print(Data.LeftDistance, 12);
	CLog::Print(Data.RightDistance, 13);
	CLog::Print(Data.LeftRotation, 14);
	CLog::Print(Data.RightRotation, 15);
	//������� �ʿ���ٸ� ������ define�� ������
#endif


}

void UCFeetComponent::Trace(FName InName, float & OutDistance, FRotator& OutRotation)
{
	FVector socktetWorldPos = OwnerCharacter->GetMesh()->GetSocketLocation(InName);
	// ������ ���� ��ǥ�� ���´�, InName���κ��� 

	float z = OwnerCharacter->GetActorLocation().Z;
	//ĳ������ ���� Z ��
	FVector start = FVector(socktetWorldPos.X, socktetWorldPos.Y, z);
	//�ش� ������ ���� x,y ��ǥ, ���̴� �÷��̾��� z�̹Ƿ� �㸮���� �����Ѵ�

	z = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	//z�� ������, ĸ�� ������Ʈ �عٴڿ��� TraceDistance ��ŭ �� ���� �İ� �Ÿ����� Trace �˻�
	FVector end = FVector(socktetWorldPos.X, socktetWorldPos.Y, z);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	//�浹 ���ø�Ͽ��� �ڽ� ĳ���͸� �־���

	FHitResult hitResult;
	//�浹 ������ ������ ����ü �̸� ����
	//TODO: ��Ȯ�� ����
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),	
		true,								//�����浹 ��, �����ϰ� ���� ���߱� ����
		ignores,
		DrawDebug,
		hitResult,
		true,								//Ʈ���̽��� ������ ������Ʈ�� ��󿡼� ����
		FLinearColor::Green,
		FLinearColor::Red
	);
	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;
	//��ȯ�� �� ���� �ʱ�ȭ

	CheckFalse(hitResult.bBlockingHit);
	//Trace ����� ���ٸ� ���⼭ ����


	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();


/*************************************
	hitResult.ImpactPoint : �浹���� hitResult.TraceEnd : Ʈ���̽� ������
	Trace�� �����κ��� �浹�� ���������� ����
	�׷��ϱ� �ּ� ���� 0�� �ִ� TraceDistance��ŭ �Ʒ��� ������ ��ġ�� ���� �������� �������
	0 => -TraceDistance ��ŭ �Ʒ��� �̵��ؾ� �Ѵ�
**************************************/

	OutDistance = length  - TraceDistance;


	/*************************************
RotationFromX�� ����ص� �ȴ�
	UKismetMathLibrary::MakeRotFromX();

**************************************/
	float roll = UKismetMathLibrary::DegAtan2(hitResult.ImpactNormal.Y, hitResult.ImpactNormal.Z);
	//Y/Z ź��Ʈ ���Լ� �� Z�࿡�� Y�������� ȸ���� -> Roll�� ȸ������� ���� ������
	//���� ����� ����
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.ImpactNormal.X, hitResult.ImpactNormal.Z);
	//X/Z ź��Ʈ ���Լ� �� Z�࿡�� X�������� ȸ���� -> pitch ȸ���� �ݴ� �����̹Ƿ� - �� �ٿ��ش�
	//������ ���ʿ� ���� Z/X�� �־��شٸ�?
	OutRotation = FRotator(pitch, 0, roll);

}

