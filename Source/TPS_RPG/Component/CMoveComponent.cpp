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
	CHECK_NULL_UOBJECT(OwnerCharacter)
	
	SetSpeed(ESpeedType::Run);
	DisableControlRotation();
}

void UCMoveComponent::OnMoveForward(float InAxis)
{
	CHECK_NULL_UOBJECT(OwnerCharacter);
	CHECK_TRUE(IsCanMove==false);

	FRotator const RotatorToMove = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector const DirectionToMove = FQuat(RotatorToMove).GetForwardVector();

	OwnerCharacter->AddMovementInput(DirectionToMove, InAxis);
}

void UCMoveComponent::OnMoveRight(float InAxis)
{
	CHECK_NULL_UOBJECT(OwnerCharacter);
	CHECK_TRUE(IsCanMove==false);

	FRotator const RotatorToMove = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector const DirectionToMove = FQuat(RotatorToMove).GetRightVector();

	OwnerCharacter->AddMovementInput(DirectionToMove, InAxis);
}

void UCMoveComponent::OnHorizontalLook(float InAxis)const
{
	CHECK_NULL_UOBJECT(OwnerCharacter);
	CHECK_TRUE(IsFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * SpeedOfMouseScroll.X * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::OnVerticalLook(float InAxis)const
{
	CHECK_NULL_UOBJECT(OwnerCharacter);
	CHECK_TRUE(IsFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * SpeedOfMouseScroll.Y * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::SetSpeed(ESpeedType InType)
{
	CHECK_NULL_UOBJECT(OwnerCharacter);
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
	CHECK_NULL_UOBJECT(OwnerCharacter);

	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMoveComponent::DisableControlRotation()const
{
	CHECK_NULL_UOBJECT(OwnerCharacter);
	
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}