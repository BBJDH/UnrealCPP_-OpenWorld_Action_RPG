

#include "Component/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Human/CHuman.h"

//#define LOG_UCFeetComponent 1
#define LOG_UCFeetComponent 0

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DrawDebug = EDrawDebugTrace::Type::None;
	IsOnTrace = true;
}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	//Cast<ACHuman>(OwnerCharacter)->StartFall.AddUFunction(this, "StartInAir");
	//Cast<ACHuman>(OwnerCharacter)->EndFall.AddUFunction(this, "EndInAir");
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(IsOnTrace);

	float MoveAmountOfLeftFeet{}, MoveAmountOfRightFeet{};		//�޾ƿ� �� ����
	FRotator RotationOfLeftFeet{}, RotationOfRightFeet{};	//�޾ƿ� �� ȸ����

	Trace(LeftSocket, MoveAmountOfLeftFeet, RotationOfLeftFeet);
	Trace(RightSocket, MoveAmountOfRightFeet, RotationOfRightFeet);

	float MoveAmountOfPelvis = FMath::Min(MoveAmountOfLeftFeet, MoveAmountOfRightFeet);
	//����߿� ���� ���� ����(����)�� ������

	/*************************************
	Data �ȿ��� ���� �߰� ���� ����, ���� �����̼� ���� ����ִ�
	(�� ���Ʒ��� �󸶳� �������� ��������� �����Ѵ�)
	Pelvis�� ��������

	Offset�� ���� ���� ���� ���̸� ������ (-TraceDistance ~ +TraceDistance) 
	Pelvis�� ���� ������ ���̸� ���
**************************************/
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, MoveAmountOfPelvis, DeltaTime, InterpSpeed);
	//��������(��Ÿ��) ������, ��ǥ��, ��Ÿ�ð�, �����ӵ�

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (MoveAmountOfLeftFeet - MoveAmountOfPelvis), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(MoveAmountOfRightFeet - MoveAmountOfPelvis), DeltaTime, InterpSpeed);
	//�Ѵ� ��Pelvis ��ŭ ���̸� �ø��ų� ����

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, RotationOfLeftFeet, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, RotationOfRightFeet, DeltaTime, InterpSpeed);
#if LOG_UCFeetComponent
	CLog::Print(Data.PelvisDistance, 11);
	CLog::Print(Data.LeftDistance, 12);
	CLog::Print(Data.RightDistance, 13);
	CLog::Print(Data.LeftRotation, 14);
	CLog::Print(Data.RightRotation, 15);
	//������� �ʿ���ٸ� ������ define�� ������
#endif


}

void UCFeetComponent::Trace(FName InSocketName, float & OutDistance, FRotator& OutRotation)
{
	FVector WorldPosOfSocktet = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	// ������ ���� ��ǥ�� ���´�, InName���κ��� 

	float StartOfVectorZ = OwnerCharacter->GetActorLocation().Z;
	//ĳ������ ���� Z ��
	FVector StartOfVector = FVector(WorldPosOfSocktet.X, WorldPosOfSocktet.Y, StartOfVectorZ);
	//�ش� ������ ���� x,y ��ǥ, ���̴� �÷��̾��� z�̹Ƿ� �㸮���� �����Ѵ�

	float EndOfVectorZ = StartOfVector.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - DistanceOfDonwFeet;
	//z�� ������, ĸ�� ������Ʈ �عٴڿ��� TraceDistance ��ŭ �� ���� �İ� �Ÿ����� Trace �˻�
	FVector EndOfVector = FVector(WorldPosOfSocktet.X, WorldPosOfSocktet.Y, EndOfVectorZ);

	TArray<AActor*> ArrayOfIgnores;
	ArrayOfIgnores.Add(OwnerCharacter);
	//�浹 ���ø�Ͽ��� �ڽ� ĳ���͸� �־���

	FHitResult HitResult;
	//�浹 ������ ������ ����ü �̸� ����
	//TODO: ��Ȯ�� ����
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		StartOfVector,
		EndOfVector,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),	
		true,								//�����浹 ��, �����ϰ� ���� ���߱� ����
		ArrayOfIgnores,
		DrawDebug,
		HitResult,
		true,								//Ʈ���̽��� ������ ������Ʈ�� ��󿡼� ����
		FLinearColor::Green,
		FLinearColor::Red
	);
	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;
	//��ȯ�� �� ���� �ʱ�ȭ

	CheckFalse(HitResult.bBlockingHit);
	//Trace ����� ���ٸ� ���⼭ ����


	float LengthOfEndToImpact = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();


/*************************************
	hitResult.ImpactPoint : �浹���� hitResult.TraceEnd : Ʈ���̽� ������
	Trace�� �����κ��� �浹�� ���������� ����
	�׷��ϱ� �ּ� ���� 0�� �ִ� TraceDistance��ŭ �Ʒ��� ������ ��ġ�� ���� �������� �������
	0 => -TraceDistance ��ŭ �Ʒ��� �̵��ؾ� �Ѵ�
**************************************/

	OutDistance = LengthOfEndToImpact + OffsetDistance - DistanceOfDonwFeet;

	/*************************************
RotationFromX�� ����ص� �ȴ�
	UKismetMathLibrary::MakeRotFromX();

**************************************/
	float OutRotationOfRoll = UKismetMathLibrary::DegAtan2(HitResult.ImpactNormal.Y, HitResult.ImpactNormal.Z);
	//Y/Z ź��Ʈ ���Լ� �� Z�࿡�� Y�������� ȸ���� -> Roll�� ȸ������� ���� ������
	//���� ����� ����
	float OutRotationOfPitch= -UKismetMathLibrary::DegAtan2(HitResult.ImpactNormal.X, HitResult.ImpactNormal.Z);
	//X/Z ź��Ʈ ���Լ� �� Z�࿡�� X�������� ȸ���� -> pitch ȸ���� �ݴ� �����̹Ƿ� - �� �ٿ��ش�
	//������ ���ʿ� ���� Z/X�� �־��شٸ�?
	OutRotation = FRotator(OutRotationOfPitch, 0, OutRotationOfRoll);

}

void UCFeetComponent::StartInAir()
{

	IsOnTrace = false;
}

void UCFeetComponent::EndInAir()
{
	IsOnTrace = true;
}

