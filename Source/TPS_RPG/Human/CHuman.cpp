

#include "CHuman.h"
#include "Global.h"

#include "CAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Component/CMontageComponent.h"

#include "Component/CFeetComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CZoomComponent.h"
#include "Component/CStateComponent.h"


ACHuman::ACHuman()
{
	Asign();
}

void ACHuman::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters params;
	params.Owner = this;

	
}

void ACHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Bind(PlayerInputComponent);

}

void ACHuman::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (this->GetVelocity().Z < -1000)
		Montage->PlayLended();

	if (EndFall.IsBound())
		EndFall.Broadcast();
}

void ACHuman::Falling()
{
	Super::Falling();
	if (StartFall.IsBound())
		StartFall.Broadcast();
}

void ACHuman::OnJumpPressed()
{
	Super::Jump();
	//���� �̺�Ʈ ���� Animinstance

	if(Super::CanJump())
	{
		switch (this->JumpCurrentCount)
		{
		case 0: Montage->PlayFirstJump(); break;
		case 1: Montage->PlaySecondJump(); break;
		}
	}

	if (StartFall.IsBound())
		StartFall.Broadcast();
}


void ACHuman::OnJumpReleased()
{
	Super::StopJumping();
}

void ACHuman::OnMoveForward(float const InAxisValue)
{
	//�����̸� InAxisValue 1, �Ĺ��̸� -1

	FRotator const rotator = FRotator(0, GetControlRotation().Yaw, 0);
	//��Ʈ�ѷ��� �ٶ󺸴� ���� ���� Yaw ���� ������
	FVector const direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�
	//Rotator(ī�޶� �ٶ󺸴� Yaw)�� Q(X,Y,Z) , GetForwardVector()�� �����Ͽ� ���̸� ���λ���

	AddMovementInput(direction, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�

	AddMovementInput(direction, InAxisValue);
}

void ACHuman::HorizontalLook(float const InAxisValue)
{
	AddControllerYawInput(InAxisValue);
}

void ACHuman::VerticalLook(float const InAxisValue)
{
	AddControllerPitchInput(InAxisValue);
}

void ACHuman::TestKeyBroadCast()
{
	if (TestKeyEvent.IsBound())
		TestKeyEvent.Broadcast();
}


void ACHuman::Asign()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montage, "Montage");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");

	//Pawn Yaw�� ��Ʈ�ѷ��� �ͼӽ�Ű�� ����
	this->bUseControllerRotationYaw = false;
	//ĳ���͹����Ʈ�� ������ ���Ӹ�ŭ �̵��������� ȸ�������� �����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MAX_WALK_SPEED;

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

/*
======================================
			SpringArm Setting
======================================
*/


	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->SocketOffset = FVector(0, 60, 0);
	SpringArm->TargetArmLength = 600;	
	SpringArm->bEnableCameraLag = true;		//ī�޶� �ε巴�� ���������
	SpringArm->CameraLagSpeed = 5;
	SpringArm->CameraRotationLagSpeed = 2;
	SpringArm->bDoCollisionTest = true;	//�������Ͽ� �ɸ��� ���ư�����
	SpringArm->bUsePawnControlRotation = true;	//Pawn���� ȸ������

/*
======================================
			Jump Setting
======================================
*/
	this->JumpMaxCount = 2;
	this->JumpMaxHoldTime = 0.2f;

}

void ACHuman::Bind(UInputComponent * const PlayerInputComponent)
{
	CheckNull(PlayerInputComponent);

	//Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACHuman::OnJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACHuman::OnJumpReleased);

	PlayerInputComponent->BindAction("GreatSword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetGreatSwordMode);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);


	//Test
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Pressed, this, &ACHuman::TestKeyBroadCast);


	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACHuman::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACHuman::OnMoveRight);
	//���� ������ �Է¿��� ���ǵ� �̸��� �̺�Ʈ ���ε�
	//����ȭ ���� �ʾƵ� �ȴ� �Լ� ������ ���ε�
	//BindAxis�� ���ڷ� float �ϳ��� �޾ƾ� �Ѵ�

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACHuman::HorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACHuman::VerticalLook);

	PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetZoomValue);
}

