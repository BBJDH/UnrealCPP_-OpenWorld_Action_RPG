

#include "CHuman.h"

#include <string>

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
	//if (this->GetCharacterMovement()->IsFalling())
	//	CLog::Print("Falling");
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
	CustomJumpCount = 0;
	if (this->GetVelocity().Z < -1000)
		Montage->PlayLended();

	EndFall();
	//AnimInstance, Status, FeetComponent ����
	//if (EndFall.IsBound() /*and Status->IsInAir()*/)
	//	EndFall.Broadcast();
}

void ACHuman::Falling()
{
	Super::Falling();

	StartFall();
	//AnimInstance, Status, FeetComponent ����... �� �þ�ٸ� �������...?
	//if (StartFall.IsBound()/* and Status->IsInAir()*/ )
	//	StartFall.Broadcast();
}

void ACHuman::OnJumpPressed()
{
	Super::Jump();
	//bPressedJump = true;


	if(Super::CanJump())
	{
		switch (this->JumpCurrentCount)
		{
		case 0: Montage->PlayFirstJump(); break;
		case 1: Montage->PlaySecondJump(); break;
		}
	}

	StartFall();
	//AnimInstance, Status, FeetComponent ����
	//if (StartFall.IsBound())
	//	StartFall.Broadcast();
}


void ACHuman::OnJumpReleased()
{
	//bPressedJump = false;
	Super::StopJumping();
}

void ACHuman::OnMoveForward(float const InAxisValue)
{
	//�����̸� InAxisValue 1, �Ĺ��̸� -1

	FRotator const rotator = FRotator(0, GetControlRotation().Yaw, 0);
	//��Ʈ�ѷ��� �ٶ󺸴� ���� ���� Yaw ���� ������
	FVector const direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�
	//Rotator(ī�޶� �ٶ󺸴� Yaw)�� Q(X,Y,Z) , GetForwardVector()�� �����Ͽ� ���̸� ���λ���

	//CLog::Print(static_cast<int>(SpringArm->bUsePawnControlRotation));
	AddMovementInput(direction, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�

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
		FVector lookAt = target - start;
		lookAt.Z = 0;

		FVector direction = FQuat( lookAt.Rotation() + data->LaunchRotation).GetForwardVector();
		direction.Normalize();

		if(data->IsLaunchAttacker)
		{
			DamageData.Attacker->LaunchCharacter(direction * data->Launch*1.02f, false, false);
			ACHuman* attacker = Cast<ACHuman>(DamageData.Attacker);

			CheckNull(attacker);

			StartFall();
			/*if (attacker->StartFall.IsBound())
				attacker->StartFall.Broadcast();*/
		}

		LaunchCharacter(direction * data->Launch, false, false);
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);
		lookAtRotation.Pitch = 0;
		lookAtRotation.Roll = 0;
		SetActorRotation(lookAtRotation);
	}

	DamageData.Attacker = nullptr;
	DamageData.Event = nullptr;
}

void ACHuman::Dead()
{
}

void ACHuman::StartFall()
{
	//AnimInstance, Status, FeetComponent ����
	UAnimInstance * animInstance =  this->GetMesh()->GetAnimInstance();
	CheckNull(animInstance);
	Cast<UCAnimInstance>(animInstance)->StartInAir();
	Status->StartInAir();
	Feet->StartInAir();

}

void ACHuman::EndFall()
{
	//AnimInstance, Status, FeetComponent ����
	UAnimInstance* animInstance = this->GetMesh()->GetAnimInstance();
	CheckNull(animInstance);
	Cast<UCAnimInstance>(animInstance)->EndInAir();
	Status->EndInAir();
	Feet->EndInAir();
}

//void ACHuman::ClearJumpInput(float DeltaTime)
//{
//	Super::ClearJumpInput(DeltaTime);
//	if (JumpKeyHoldTime >= GetJumpMaxHoldTime())
//	{
//		CLog::Print("Clear!!");
//	}
//}


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

	//Pawn Yaw�� ��Ʈ�ѷ��� �ͼӽ�Ű�� ����
	this->bUseControllerRotationYaw = false;
	//ĳ���͹����Ʈ�� ������ ���Ӹ�ŭ �̵��������� ȸ�������� �����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;

	USkeletalMesh* mesh = nullptr;

	//���۷����� ���� ��������
	//CHelpers::GetAsset<USkeletalMesh>(&mesh, "");
	//GetMesh()->SetSkeletalMesh(mesh);
	//��ġ ����, �Ʒ��� 90, 
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); //Pitch Yaw Roll

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'");

	GetMesh()->SetAnimClass(animInstance);
	//���� �Լ��� ���������� ���Լ��� ȣ�� GetMesh()->SetAnimInstanceClass(animInstance);
}

