

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


ACHuman::ACHuman() 
{
	CHECK_NULL_UOBJECT(GetCapsuleComponent());
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CHECK_NULL_UOBJECT(SpringArm);

	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	CHECK_NULL_UOBJECT(Camera);

	Camera->SetupAttachment(SpringArm);

	//Create Custom Component
	Weapon = this->CreateDefaultSubobject<UCWeaponComponent>("Weapon");
	CHECK_NULL_UOBJECT(Weapon);

	State = this->CreateDefaultSubobject<UCStateComponent>("State");
	CHECK_NULL_UOBJECT(State);

	Status = this->CreateDefaultSubobject<UCStatusComponent>("Status");
	CHECK_NULL_UOBJECT(Status);

	Montage = this->CreateDefaultSubobject<UCMontageComponent>("Montage");
	CHECK_NULL_UOBJECT(Montage);

	Feet = this->CreateDefaultSubobject<UCFeetComponent>("Feet");
	CHECK_NULL_UOBJECT(Feet);

	Move = this->CreateDefaultSubobject<UCMoveComponent>("Move");
	CHECK_NULL_UOBJECT(Move);


	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//CheckNullUObject(GetMesh()->SkeletalMesh);

	ConstructorHelpers::FClassFinder<UCAnimInstance> const AssetFound(*FString( "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'"));
	CHECK_NULL_UOBJECT(AssetFound.Class);

	GetMesh()->SetAnimClass(AssetFound.Class);
}

void ACHuman::BeginPlay()
{
	Super::BeginPlay();
	CHECK_NULL_UOBJECT(State);
	State->OnStateTypeChanged.AddDynamic(this, &ACHuman::OnStateTypeChanged);
}

void ACHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACHuman::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	float const AmountOfDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CHECK_NULL_UOBJECT_RESULT(State, AmountOfDamage);
	CHECK_NULL_UOBJECT_RESULT(EventInstigator->GetPawn(), AmountOfDamage);
	
	DamageData.Amount = AmountOfDamage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Event = (FHitDamageEvent*)&DamageEvent;

	State->SetGetHitMode();
	return AmountOfDamage;
}


void ACHuman::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CHECK_NULL_UOBJECT(Montage);
	CustomJumpCount = 0;
	if (this->GetVelocity().Z < -1000)
	{
		Montage->PlayLended();
	}

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
	CHECK_NULL_UOBJECT(Montage);
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

// void ACHuman::OnDodgeAction() const
// {
// 	CheckNullUObject(Montage);
// 	Montage->PlayFirstJump(); 
// }


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
	CHECK_NULL(DamageData.Event);
	CHECK_NULL(DamageData.Event->HitData);
	CHECK_NULL_UOBJECT(Status);
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

//캐릭터 Dead상태 진입시 Call, 충돌 변경 및 몽타주 플레이
void ACHuman::Dead() const
{
	CHECK_NULL_UOBJECT(GetCapsuleComponent());
	CHECK_NULL_UOBJECT(GetController());
	CHECK_NULL_UOBJECT(Montage);
	GetCapsuleComponent()->SetCollisionProfileName("RagdollWithNoCam");
	Montage->PlayDead();

	GetController()->UnPossess();
}

//Normalized Return
FVector ACHuman::GetVectorLookAtActor(AActor const* InActor) const
{
	CHECK_NULL_UOBJECT_RESULT(InActor,FVector());
	
	FVector const LocationOfSelf = GetActorLocation();
	FVector const LocationOfDest= InActor->GetActorLocation();
	FVector LookAtDest = LocationOfDest - LocationOfSelf;
	LookAtDest.Normalize();
	return LookAtDest;
}

//Yaw 회전만 사용
void ACHuman::SetActorRotation2D(FRotator LookAtRotator)
{
	LookAtRotator.Pitch = 0;
	LookAtRotator.Roll = 0;
	SetActorRotation(LookAtRotator);
}

//Broadcast Falling
void ACHuman::StartFall() const
{
	CHECK_NULL_UOBJECT(Status);
	CHECK_NULL_UOBJECT(Feet);
	CHECK_NULL_UOBJECT(Weapon);

	UCAnimInstance * SelfAnimInstance =  Cast<UCAnimInstance>(this->GetMesh()->GetAnimInstance());
	CHECK_NULL_UOBJECT(SelfAnimInstance);
	
	SelfAnimInstance->StartInAir();
	Status->StartInAir();
	Feet->StartInAir();
	Weapon->InitComboIndex();

}

//Broadcast Landed
void ACHuman::EndFall() const
{
	CHECK_NULL_UOBJECT(Status);
	CHECK_NULL_UOBJECT(Feet);
	CHECK_NULL_UOBJECT(Weapon);

	UCAnimInstance * SelfAnimInstance =  Cast<UCAnimInstance>(this->GetMesh()->GetAnimInstance());
	CHECK_NULL_UOBJECT(SelfAnimInstance);
	
	SelfAnimInstance->EndInAir();
	Status->EndInAir();
	Feet->EndInAir();
	Weapon->InitComboIndex();

}

//Notify Call, 죽음 처리
void ACHuman::NotifyDead()
{
	CHECK_NULL_UOBJECT(Weapon);

	Weapon->DestroyWeapons();
	Destroy();
}

