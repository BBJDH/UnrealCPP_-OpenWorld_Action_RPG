

#include "Component/CZoomComponent.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"

UCZoomComponent::UCZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCZoomComponent::BeginPlay()
{
	Super::BeginPlay();
	SpringArm = Cast<USpringArmComponent>(GetOwner()->GetComponentByClass(USpringArmComponent::StaticClass()));
	//SpringArm = CHelpers::GetComponent<USpringArmComponent>(GetOwner());
	CHECK_NULL_UOBJECT(SpringArm);
	
	DestValueOfZoomLength = SpringArm->TargetArmLength;
}

void UCZoomComponent::InterpCurrentZoomLength(float const DeltaTime)
{
	CHECK_NULL_UOBJECT(SpringArm);
	CHECK_TRUE(UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, DestValueOfZoomLength));

	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, DestValueOfZoomLength, DeltaTime, ZoomSpeedOfInterpolation);
}


void UCZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InterpCurrentZoomLength(DeltaTime);
}

void UCZoomComponent::SetZoomValue(float InValue)
{
	DestValueOfZoomLength += (ZoomSpeed * InValue);
	DestValueOfZoomLength = FMath::Clamp(DestValueOfZoomLength, ZoomRange.X, ZoomRange.Y);
}