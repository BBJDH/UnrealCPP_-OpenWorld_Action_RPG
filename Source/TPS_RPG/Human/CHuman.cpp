

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
#include "Component/CWeaponComponent.h"
#include "Component/CZoomComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Weapon/CWeaponStructures.h"

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
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Amount = damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Event = (FHitDamageEvent*)&DamageEvent;

	State->SetHittedMode();

	return damage;
}


void ACHuman::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (this->GetVelocity().Z < -1000)
		Montage->PlayLended();

	//AnimInstance, Status 전파
	if (EndFall.IsBound() and Status->IsInAir())
		EndFall.Broadcast();
}

void ACHuman::Falling()
{
	Super::Falling();

	//AnimInstance, Status 전파
	if (StartFall.IsBound() and Status->IsInAir() )
		StartFall.Broadcast();
}

void ACHuman::OnJumpPressed()
{
	Super::Jump();
	//점프 이벤트 전달 Animinstance

	if(Super::CanJump())
	{
		switch (this->JumpCurrentCount)
		{
		case 0: Montage->PlayFirstJump(); break;
		case 1: Montage->PlaySecondJump(); break;
		}
	}

	//AnimInstance, Status 전파
	if (StartFall.IsBound())
		StartFall.Broadcast();
}


void ACHuman::OnJumpReleased()
{
	Super::StopJumping();
}

void ACHuman::OnMoveForward(float const InAxisValue)
{
	//전방이면 InAxisValue 1, 후방이면 -1

	FRotator const rotator = FRotator(0, GetControlRotation().Yaw, 0);
	//컨트롤러가 바라보는 전방 벡터 Yaw 값을 가져옴
	FVector const direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();//포워드 벡터이지만 노멀라이즈가 필요
	//Rotator(카메라가 바라보는 Yaw)의 Q(X,Y,Z) , GetForwardVector()는 외적하여 길이를 늘인상태

	AddMovementInput(direction, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();//포워드 벡터이지만 노멀라이즈가 필요

	AddMovementInput(direction, InAxisValue);
}



void ACHuman::OnStateTypeChanged(EStateType const InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}

void ACHuman::Hitted()
{
	Status->Damage(DamageData.Amount);
	DamageData.Amount = 0;

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();

		return;
	}


	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* data = DamageData.Event->HitData;

		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());
		data->PlaySound(this);
		data->PlayEffect(GetWorld(), GetActorLocation());


		FVector start = GetActorLocation();
		FVector target = DamageData.Attacker->GetActorLocation();
		FVector direction = target - start;
		direction.Normalize();

		LaunchCharacter(-direction * data->Launch, false, false);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	}

	DamageData.Attacker = nullptr;
	DamageData.Event = nullptr;
}

void ACHuman::Dead()
{
}


void ACHuman::Asign()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");

	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montage, "Montage");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");

	//Pawn Yaw를 컨트롤러에 귀속시키지 않음
	this->bUseControllerRotationYaw = false;
	//캐릭터무브먼트에 지정된 가속만큼 이동방향으로 회전방향을 잡아줌
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MAX_WALK_SPEED;

	USkeletalMesh* mesh = nullptr;

	//레퍼런스로 에셋 가져오기
	//CHelpers::GetAsset<USkeletalMesh>(&mesh, "");
	//GetMesh()->SetSkeletalMesh(mesh);
	//위치 조정, 아래로 90, 
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); //Pitch Yaw Roll

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'");
	GetMesh()->SetAnimClass(animInstance);
}

