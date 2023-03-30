

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
	CheckNull(OwnerCharacter);
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(IsOnTrace);

	float MoveAmountOfLeftFeet{}, MoveAmountOfRightFeet{};		
	FRotator RotationOfLeftFeet{}, RotationOfRightFeet{};	

	//발과 지면 사이 거리 측정
	Trace(LeftSocketName, MoveAmountOfLeftFeet, RotationOfLeftFeet);
	Trace(RightSocketName, MoveAmountOfRightFeet, RotationOfRightFeet);
	float MoveAmountOfPelvis = FMath::Min(MoveAmountOfLeftFeet, MoveAmountOfRightFeet);

	//현 시점의 IK데이터 보간, 갱신
	CurrentIKData.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(CurrentIKData.PelvisDistance.Z, MoveAmountOfPelvis, DeltaTime, InterpSpeed);
	CurrentIKData.LeftDistance.X = UKismetMathLibrary::FInterpTo(CurrentIKData.LeftDistance.X, (MoveAmountOfLeftFeet - MoveAmountOfPelvis), DeltaTime, InterpSpeed);
	CurrentIKData.RightDistance.X = UKismetMathLibrary::FInterpTo(CurrentIKData.RightDistance.X, -(MoveAmountOfRightFeet - MoveAmountOfPelvis), DeltaTime, InterpSpeed);
	CurrentIKData.LeftRotation = UKismetMathLibrary::RInterpTo(CurrentIKData.LeftRotation, RotationOfLeftFeet, DeltaTime, InterpSpeed);
	CurrentIKData.RightRotation = UKismetMathLibrary::RInterpTo(CurrentIKData.RightRotation, RotationOfRightFeet, DeltaTime, InterpSpeed);


#if LOG_UCFeetComponent
	CLog::Print(CurrentIKData.PelvisDistance, 11);
	CLog::Print(CurrentIKData.LeftDistance, 12);
	CLog::Print(CurrentIKData.RightDistance, 13);
	CLog::Print(CurrentIKData.LeftRotation, 14);
	CLog::Print(CurrentIKData.RightRotation, 15);
#endif


}

//지정된 Socket의 캐릭터 허리높이부터 Socket아래 DistanceOfDonwFeet 깊이 만큼 탐색하여
//발이 움직여야할 거리와 회전량를 반환 (OutDistance, OutRotation)
void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation)
{
	FVector WorldPosOfSocktet = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);

	float StartOfVectorZ = OwnerCharacter->GetActorLocation().Z;
	FVector StartOfVector = FVector(WorldPosOfSocktet.X, WorldPosOfSocktet.Y, StartOfVectorZ);
	float EndOfVectorZ = StartOfVector.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - DistanceOfDonwFeet;
	FVector EndOfVector = FVector(WorldPosOfSocktet.X, WorldPosOfSocktet.Y, EndOfVectorZ);

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

	CheckFalse(HitResult.bBlockingHit);

	float LengthOfEndToImpact = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
	float OutRotationOfRoll = UKismetMathLibrary::DegAtan2(HitResult.ImpactNormal.Y, HitResult.ImpactNormal.Z);
	float OutRotationOfPitch = -UKismetMathLibrary::DegAtan2(HitResult.ImpactNormal.X, HitResult.ImpactNormal.Z);

	OutDistance = LengthOfEndToImpact + OffsetDistance - DistanceOfDonwFeet;
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

