

#include "CHuman.h"
#include "Global.h"

#include "CAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/CMontageComponent.h"

#include "Component/CFeetComponent.h"
#include "Component/CMoveComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Weapon/CWeaponStructures.h"

DEFINE_LOG_CATEGORY_STATIC(GameProject, Display, All)


ACHuman::ACHuman() 
{
	Asign();
}

void ACHuman::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACHuman::OnStateTypeChanged);
}

void ACHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACHuman::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float AmountOfDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Amount = AmountOfDamage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Event = (FHitDamageEvent*)&DamageEvent;

	State->SetGetHitMode();
	return AmountOfDamage;
}


void ACHuman::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CustomJumpCount = 0;
	if (this->GetVelocity().Z < -1000)
		Montage->PlayLended();

	EndFall();

}

void ACHuman::Falling()
{
	Super::Falling();

	StartFall();
}

void ACHuman::OnJumpPressed()
{
	Super::Jump();

	if(Super::CanJump())
	{
		switch (this->JumpCurrentCount)
		{
		case 0: Montage->PlayFirstJump(); break;
		case 1: Montage->PlaySecondJump(); break;
		}
	}

	StartFall();
}


void ACHuman::OnJumpReleased()
{
	Super::StopJumping();
}

//void ACHuman::OnMoveForward(float const InAxisValue)
//{
//	FRotator const RotaterOfView = FRotator(0, GetControlRotation().Yaw, 0);
//	FVector const DirectionOfForward = FQuat(RotaterOfView).GetForwardVector().GetSafeNormal2D();
//
//	AddMovementInput(DirectionOfForward, InAxisValue);
//}
//
//void ACHuman::OnMoveRight(float const InAxisValue)
//{
//	FRotator const RotaterOfView = FRotator(0, GetControlRotation().Yaw, 0);
//	FVector const DirectionORight = FQuat(RotaterOfView).GetRightVector().GetSafeNormal2D();
//
//	AddMovementInput(DirectionORight, InAxisValue);
//}

//캐릭터 상태 변화 감지
void ACHuman::OnStateTypeChanged(EStateType const InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::GetHit: GetHit(); break;
	case EStateType::Dead: Dead(); break;
	}
}

//데미지 이벤트를 받아 처리
void ACHuman::GetHit()
{
	CheckNull(DamageData.Event);
	CheckNull(DamageData.Event->HitData);
	
	Status->Damage(DamageData.Amount);
	DamageData.Amount = 0;

	FHitData* HitdataOfEnemyWeapon = DamageData.Event->HitData;

	HitdataOfEnemyWeapon->PlayMontage(this);
	HitdataOfEnemyWeapon->PlayHitStop(GetWorld());
	HitdataOfEnemyWeapon->PlaySound(this);
	HitdataOfEnemyWeapon->PlayEffect(GetWorld(), GetActorLocation());

	FVector LookAtAttacker = GetVectorLookAtActor(DamageData.Attacker);
	LookAtAttacker.Z = 0;
	
	HitdataOfEnemyWeapon->HitLaunch(this, LookAtAttacker.Rotation(), DamageData.Attacker);
	
	SetActorRotation2D(LookAtAttacker.Rotation());

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return;
	}

	DamageData.Attacker = nullptr;
	DamageData.Event = nullptr;
}

void ACHuman::Dead()
{
	CheckNull(GetCapsuleComponent());
	CheckNull(GetController());
	GetCapsuleComponent()->SetCollisionProfileName("RagdollWithNoCam");
	Montage->PlayDead();

	GetController()->UnPossess();
}

//Normalized Return
FVector ACHuman::GetVectorLookAtActor(AActor const* InActor)
{
	FVector const LocationOfSelf = GetActorLocation();
	FVector const LocationOfDest= InActor->GetActorLocation();
	FVector LookAtDest = LocationOfDest - LocationOfSelf;
	LookAtDest.Normalize();
	return LookAtDest;
}

void ACHuman::SetActorRotation2D(FRotator LookAtRotator)
{
	LookAtRotator.Pitch = 0;
	LookAtRotator.Roll = 0;
	SetActorRotation(LookAtRotator);
}

//Broadcast Falling
void ACHuman::StartFall()
{
	UAnimInstance * SelfAnimInstance =  this->GetMesh()->GetAnimInstance();
	CheckNull(SelfAnimInstance);
	Cast<UCAnimInstance>(SelfAnimInstance)->StartInAir();
	Status->StartInAir();
	Feet->StartInAir();
	Weapon->InitComboIndex();

}

//Broadcast Landed
void ACHuman::EndFall()
{
	UAnimInstance* SelfAnimInstance = this->GetMesh()->GetAnimInstance();
	CheckNull(SelfAnimInstance);
	Cast<UCAnimInstance>(SelfAnimInstance)->EndInAir();
	Status->EndInAir();
	Feet->EndInAir();
	Weapon->InitComboIndex();

}

void ACHuman::NotifyDead()
{
	Weapon->DestroyWeapons();
	Destroy();
}

void ACHuman::Asign()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	//Create Custom Component
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montage, "Montage");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");
	CHelpers::CreateActorComponent<UCMoveComponent>(this, &Move, "Move");


	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); 

	TSubclassOf<UCAnimInstance> ClassOfAnimInstance;
	CHelpers::GetClass<UCAnimInstance>(&ClassOfAnimInstance, "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'");

	GetMesh()->SetAnimClass(ClassOfAnimInstance);
}

