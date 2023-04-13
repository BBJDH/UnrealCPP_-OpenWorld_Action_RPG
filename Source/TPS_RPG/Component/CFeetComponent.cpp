

#include "Component/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

//#define LOG_UCFeetComponent 1
#define LOG_UCFeetComponent 0

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DrawDebug = EDrawDebugTrace::Type::ForOneFrame;
	IsOnTrace = true;
}

void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNullUObject(OwnerCharacter);
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTrue(IsOnTrace==false)

	float MoveAmountOfLeftFeet{}, MoveAmountOfRightFeet{};		
	FRotator RotationOfLeftFeet{}, RotationOfRightFeet{};	

	//�߰� ���� ���� �Ÿ� ����
	Trace(LeftSocketName, MoveAmountOfLeftFeet, RotationOfLeftFeet);
	Trace(RightSocketName, MoveAmountOfRightFeet, RotationOfRightFeet);
	float const MoveAmountOfPelvis = FMath::Min(MoveAmountOfLeftFeet, MoveAmountOfRightFeet);

	//�� ������ IK������ ����, ����
	CurrentIKData.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(CurrentIKData.PelvisDistance.Z, MoveAmountOfPelvis, DeltaTime, InterpolationSpeed);
	CurrentIKData.LeftDistance.X = UKismetMathLibrary::FInterpTo(CurrentIKData.LeftDistance.X, (MoveAmountOfLeftFeet - MoveAmountOfPelvis), DeltaTime, InterpolationSpeed);
	CurrentIKData.RightDistance.X = UKismetMathLibrary::FInterpTo(CurrentIKData.RightDistance.X, -(MoveAmountOfRightFeet - MoveAmountOfPelvis), DeltaTime, InterpolationSpeed);
	CurrentIKData.LeftRotation = UKismetMathLibrary::RInterpTo(CurrentIKData.LeftRotation, RotationOfLeftFeet, DeltaTime, InterpolationSpeed);
	CurrentIKData.RightRotation = UKismetMathLibrary::RInterpTo(CurrentIKData.RightRotation, RotationOfRightFeet, DeltaTime, InterpolationSpeed);


#if LOG_UCFeetComponent
	CLog::Print(CurrentIKData.PelvisDistance, 11);
	CLog::Print(CurrentIKData.LeftDistance, 12);
	CLog::Print(CurrentIKData.RightDistance, 13);
	CLog::Print(CurrentIKData.LeftRotation, 14);
	CLog::Print(CurrentIKData.RightRotation, 15);
#endif


}

//������ Socket�� ĳ���� �㸮���̺��� Socket�Ʒ� DistanceOfDonwFeet ���� ��ŭ Ž���Ͽ�
//���� ���������� �Ÿ��� ȸ������ ��ȯ (OutDistance, OutRotation)
void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation) const
{
	CheckNullUObject(OwnerCharacter);
	CheckNullUObject(OwnerCharacter->GetMesh());

	FVector const WorldPosOfSocket = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);

	float const StartOfVectorZ = OwnerCharacter->GetActorLocation().Z;
	FVector const StartOfVector = FVector(WorldPosOfSocket.X, WorldPosOfSocket.Y, StartOfVectorZ);
	float const EndOfVectorZ = StartOfVector.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - DistanceOfDownFeet;
	FVector const EndOfVector = FVector(WorldPosOfSocket.X, WorldPosOfSocket.Y, EndOfVectorZ);

	TArray<AActor*> ArrayOfIgnores;
	ArrayOfIgnores.Add(OwnerCharacter);

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		StartOfVector,
		EndOfVector,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,								
		ArrayOfIgnores,
		DrawDebug,
		HitResult,
		true,								
		FLinearColor::Green,
		FLinearColor::Red
	);

	//Initialize
	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;

	CheckTrue(HitResult.bBlockingHit==false)

	float const LengthOfEndToImpact = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
	float const OutRotationOfRoll = UKismetMathLibrary::DegAtan2(HitResult.ImpactNormal.Y, HitResult.ImpactNormal.Z);
	float const OutRotationOfPitch = -UKismetMathLibrary::DegAtan2(HitResult.ImpactNormal.X, HitResult.ImpactNormal.Z);

	OutDistance = LengthOfEndToImpact + OffsetDistance - DistanceOfDownFeet;
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

