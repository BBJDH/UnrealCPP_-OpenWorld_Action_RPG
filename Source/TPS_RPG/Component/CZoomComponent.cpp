

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
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(GetOwner());
	CheckNull(SpringArm);
	DestValueOfZoomLength = SpringArm->TargetArmLength;
}

void UCZoomComponent::InterpCurrentZoomLength(float const DeltaTime)
{
	CheckNull(SpringArm);
	CheckTrue(UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, DestValueOfZoomLength));
	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, DestValueOfZoomLength, DeltaTime, ZoomInterpSpeed);
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