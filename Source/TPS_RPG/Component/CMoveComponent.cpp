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

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	SetSpeed(ESpeedType::Run);
	DisableControlRotation();
}

void UCMoveComponent::OnMoveForward(float InAxis)
{
	CheckTrue(IsCanMove==false);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMoveComponent::OnMoveRight(float InAxis)
{
	CheckTrue(IsCanMove==false);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMoveComponent::OnHorizontalLook(float InAxis)const
{
	CheckTrue(IsFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * SpeedOfMouseScroll.X * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::OnVerticalLook(float InAxis)const
{
	CheckTrue(IsFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * SpeedOfMouseScroll.Y * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed[static_cast<int32>(InType)];
}

void UCMoveComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMoveComponent::OffSprint()
{
	SetSpeed(ESpeedType::Run);
}

void UCMoveComponent::EnableControlRotation()const
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMoveComponent::DisableControlRotation()const
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}