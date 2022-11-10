

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

	float leftDistance, rightDistance;		//받아올 발 간격
	FRotator leftRotation, rightRotation;	//받아올 발 회전각

	Trace(LeftSocket, leftDistance, leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);
	//양발중에 가장 작은 길이(높이)를 가져옴

	/*************************************
	Data 안에는 현재 발과 땅의 간격, 현재 로테이션 값이 들어있다
	(즉 위아래로 얼마나 움직여서 출력할지를 결정한다)
	Pelvis도 마찬가지

	Offset을 통해 가장 작은 높이를 가져와 (-TraceDistance ~ +TraceDistance) 
	Pelvis를 먼저 조정해 높이를 잡고
**************************************/
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
	//선형보간(델타식) 변동값, 목표값, 델타시간, 보간속도

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	//둘다 위Pelvis 만큼 높이를 올리거나 낮춤

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
#if LOG_UCFeetComponent
	CLog::Print(Data.PelvisDistance, 11);
	CLog::Print(Data.LeftDistance, 12);
	CLog::Print(Data.RightDistance, 13);
	CLog::Print(Data.LeftRotation, 14);
	CLog::Print(Data.RightRotation, 15);
	//디버깅이 필요없다면 맨위의 define을 지우자
#endif


}

void UCFeetComponent::Trace(FName InName, float & OutDistance, FRotator& OutRotation)
{
	FVector socktetWorldPos = OwnerCharacter->GetMesh()->GetSocketLocation(InName);
	// 소켓의 월드 좌표를 얻어온다, InName으로부터 

	float z = OwnerCharacter->GetActorLocation().Z;
	//캐릭터의 월드 Z 값
	FVector start = FVector(socktetWorldPos.X, socktetWorldPos.Y, z);
	//해당 소켓의 월드 x,y 좌표, 높이는 플레이어의 z이므로 허리부터 시작한다

	z = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	//z를 재정의, 캡슐 컴포넌트 밑바닥에서 TraceDistance 만큼 더 땅을 파고간 거리까지 Trace 검사
	FVector end = FVector(socktetWorldPos.X, socktetWorldPos.Y, z);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	//충돌 무시목록에는 자신 캐릭터를 넣었다

	FHitResult hitResult;
	//충돌 정보를 가져올 구조체 미리 생성
	//TODO: 정확도 개선
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),	
		true,								//복합충돌 켬, 정밀하게 발을 맞추기 위해
		ignores,
		DrawDebug,
		hitResult,
		true,								//트레이스를 수행할 컴포넌트도 대상에서 제외
		FLinearColor::Green,
		FLinearColor::Red
	);
	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;
	//반환할 두 값을 초기화

	CheckFalse(hitResult.bBlockingHit);
	//Trace 결과가 없다면 여기서 종료


	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();


/*************************************
	hitResult.ImpactPoint : 충돌지점 hitResult.TraceEnd : 트래이스 끝지점
	Trace의 끝으로부터 충돌된 지점까지의 길이
	그러니까 최소 길이 0은 최대 TraceDistance만큼 아래로 내려간 위치에 대한 길이임을 기억하자
	0 => -TraceDistance 만큼 아래로 이동해야 한다
**************************************/

	OutDistance = length  - TraceDistance;


	/*************************************
RotationFromX를 사용해도 된다
	UKismetMathLibrary::MakeRotFromX();

**************************************/
	float roll = UKismetMathLibrary::DegAtan2(hitResult.ImpactNormal.Y, hitResult.ImpactNormal.Z);
	//Y/Z 탄젠트 역함수 즉 Z축에서 Y축으로의 회전값 -> Roll과 회전방향과 같이 같아짐
	//외적 방향과 동일
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.ImpactNormal.X, hitResult.ImpactNormal.Z);
	//X/Z 탄젠트 역함수 즉 Z축에서 X축으로의 회전값 -> pitch 회전과 반대 방향이므로 - 를 붙여준다
	//하지만 애초에 값을 Z/X로 넣어준다면?
	OutRotation = FRotator(pitch, 0, roll);

}

