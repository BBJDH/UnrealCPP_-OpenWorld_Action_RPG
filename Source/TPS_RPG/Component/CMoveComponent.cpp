#include "Component/CMoveComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMoveComponent::UCMoveComponent()
{
	
}

void UCMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	
	SetSpeed(ESpeedType::Run);
	DisableControlRotation();
}

void UCMoveComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, Owner->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	Owner->AddMovementInput(direction, InAxis);
}

void UCMoveComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, Owner->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	Owner->AddMovementInput(direction, InAxis);
}

void UCMoveComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	Owner->AddControllerYawInput(InAxis * MouseSpeed.X * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	Owner->AddControllerPitchInput(InAxis * MouseSpeed.Y * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::SetSpeed(ESpeedType InType)
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMoveComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMoveComponent::OffSprint()
{
	SetSpeed(ESpeedType::Run);
}

void UCMoveComponent::EnableControlRotation()
{
	Owner->bUseControllerRotationYaw = true;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMoveComponent::DisableControlRotation()
{
	Owner->bUseControllerRotationYaw = false;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
}