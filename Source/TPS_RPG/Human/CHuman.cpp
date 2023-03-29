

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

	//if(Status->HasBegunPlay()==false)
	//{
	//	
	//}
	
	//FString StrToString = "CPP_Human";
	//UE_LOG(GameProject, Display, TEXT("%s"), *StrToString);

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
	float AmountOfDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Amount = AmountOfDamage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Event = (FHitDamageEvent*)&DamageEvent;

	State->SetHittedMode();
	return AmountOfDamage;
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

	FRotator const RotaterOfView = FRotator(0, GetControlRotation().Yaw, 0);
	//��Ʈ�ѷ��� �ٶ󺸴� ���� ���� Yaw ���� ������
	FVector const DirectionOfForward = FQuat(RotaterOfView).GetForwardVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�
	//Rotator(ī�޶� �ٶ󺸴� Yaw)�� Q(X,Y,Z) , GetForwardVector()�� �����Ͽ� ���̸� ���λ���

	//CLog::Print(static_cast<int>(SpringArm->bUsePawnControlRotation));
	AddMovementInput(DirectionOfForward, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const RotaterOfView = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const DirectionORight = FQuat(RotaterOfView).GetRightVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�

	AddMovementInput(DirectionORight, InAxisValue);
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
	CheckNull(DamageData.Event);
	CheckNull(DamageData.Event->HitData);
	//CheckTrue(State->IsDeadMode())
	
	
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

void ACHuman::StartFall()
{
	//AnimInstance, Status, FeetComponent ����
	UAnimInstance * SelfAnimInstance =  this->GetMesh()->GetAnimInstance();
	CheckNull(SelfAnimInstance);
	Cast<UCAnimInstance>(SelfAnimInstance)->StartInAir();
	Status->StartInAir();
	Feet->StartInAir();
	Weapon->InitComboIndex();

}

void ACHuman::EndFall()
{
	//AnimInstance, Status, FeetComponent ����
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

	TSubclassOf<UCAnimInstance> ClassOfAnimInstance;
	CHelpers::GetClass<UCAnimInstance>(&ClassOfAnimInstance, "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'");

	GetMesh()->SetAnimClass(ClassOfAnimInstance);
	//���� �Լ��� ���������� ���Լ��� ȣ�� GetMesh()->SetAnimInstanceClass(animInstance);
}

