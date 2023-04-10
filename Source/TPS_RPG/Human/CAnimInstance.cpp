// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
//#define LOG_UCFeetComponent 1
#define LOG_UCFeetComponent 0

//void UCAnimInstance::ToggleIK()
//{
//	IsOnFeetIK = !IsOnFeetIK;
//	CLog::Print(IsOnFeetIK);
//}

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	UE_LOG(GameProject, Display, TEXT("FunctionCall : %s, LINE : %s"), *FString(__FUNCTION__), *FString::FromInt(__LINE__));

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	CheckNullUObject(OwnerCharacter);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullUObject(WeaponComponent);

	WeaponComponent->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNullUObject(OwnerCharacter);
	
	Speed = OwnerCharacter->GetVelocity().Size2D();

	//UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);
	UCFeetComponent* FeetIKComponent = Cast<UCFeetComponent>(OwnerCharacter->GetComponentByClass(UCFeetComponent::StaticClass()));

	CheckNullUObject(FeetIKComponent);
	if (FeetIKComponent != nullptr && OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero())
	{
		IsOnFeetIK = true;
		FeetIKData = FeetIKComponent->GetData();
#if LOG_UCFeetComponent
		CLog::Print(FeetIKData.PelvisDistance, 11);
		CLog::Print(FeetIKData.LeftDistance, 12);
		CLog::Print(FeetIKData.RightDistance, 13);
		CLog::Print(FeetIKData.LeftRotation, 14);
		CLog::Print(FeetIKData.RightRotation, 15);

#endif
	}
	else
	{
		IsOnFeetIK = false;
	}
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCAnimInstance::StartInAir()
{
	InAir = true;
}

void UCAnimInstance::EndInAir()
{
	InAir = false;
}
