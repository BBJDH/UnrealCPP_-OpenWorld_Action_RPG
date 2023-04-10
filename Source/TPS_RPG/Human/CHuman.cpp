

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
	CheckNullUObject(GetCapsuleComponent());
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CheckNullUObject(SpringArm);

	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	CheckNullUObject(Camera);

	Camera->SetupAttachment(SpringArm);

	//Create Custom Component
	Weapon = this->CreateDefaultSubobject<UCWeaponComponent>("Weapon");
	CheckNullUObject(Weapon);

	State = this->CreateDefaultSubobject<UCStateComponent>("State");
	CheckNullUObject(State);

	Status = this->CreateDefaultSubobject<UCStatusComponent>("Status");
	CheckNullUObject(Status);

	Montage = this->CreateDefaultSubobject<UCMontageComponent>("Montage");
	CheckNullUObject(Montage);

	Feet = this->CreateDefaultSubobject<UCFeetComponent>("Feet");
	CheckNullUObject(Feet);

	Move = this->CreateDefaultSubobject<UCMoveComponent>("Move");
	CheckNullUObject(Move);


	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//CheckNullUObject(GetMesh()->SkeletalMesh);

	ConstructorHelpers::FClassFinder<UCAnimInstance> const AssetFound(*FString( "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'"));
	CheckNullUObject(AssetFound.Class);

	GetMesh()->SetAnimClass(AssetFound.Class);
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

	float const AmountOfDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CheckNullUObjectResult(State, AmountOfDamage);
	CheckNullUObjectResult(EventInstigator->GetPawn(), AmountOfDamage);
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
	CheckNullUObject(Status);
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

void ACHuman::Dead() const
{
	CheckNullUObject(GetCapsuleComponent());
	CheckNullUObject(GetController());
	GetCapsuleComponent()->SetCollisionProfileName("RagdollWithNoCam");
	Montage->PlayDead();

	GetController()->UnPossess();
}

//Normalized Return
FVector ACHuman::GetVectorLookAtActor(AActor const* InActor) const
{
	CheckNullUObjectResult(InActor,FVector());
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
void ACHuman::StartFall() const
{
	CheckNullUObject(Status);
	CheckNullUObject(Feet);
	CheckNullUObject(Weapon);

	UAnimInstance * SelfAnimInstance =  this->GetMesh()->GetAnimInstance();
	CheckNullUObject(SelfAnimInstance);
	Cast<UCAnimInstance>(SelfAnimInstance)->StartInAir();

	Status->StartInAir();
	Feet->StartInAir();
	Weapon->InitComboIndex();

}

//Broadcast Landed
void ACHuman::EndFall() const
{
	CheckNullUObject(Status);
	CheckNullUObject(Feet);
	CheckNullUObject(Weapon);

	UAnimInstance* SelfAnimInstance = this->GetMesh()->GetAnimInstance();
	CheckNullUObject(SelfAnimInstance);
	Cast<UCAnimInstance>(SelfAnimInstance)->EndInAir();

	Status->EndInAir();
	Feet->EndInAir();
	Weapon->InitComboIndex();

}

void ACHuman::NotifyDead()
{
	CheckNullUObject(Weapon);

	Weapon->DestroyWeapons();
	Destroy();
}

