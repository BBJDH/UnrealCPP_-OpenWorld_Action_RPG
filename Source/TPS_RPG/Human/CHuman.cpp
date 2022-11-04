

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

ACHuman::ACHuman()
{
	Asign();
}

void ACHuman::BeginPlay()
{
	Super::BeginPlay();

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



void ACHuman::Asign()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montage, "Montage");

	//Pawn Yaw�� ��Ʈ�ѷ��� �ͼӽ�Ű�� ����
	this->bUseControllerRotationYaw = false;
	//ĳ���͹����Ʈ�� ������ ���Ӹ�ŭ �̵��������� ȸ�������� �����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MAX_WALK_SPEED;

	USkeletalMesh* mesh = nullptr;

	//���۷����� ���� ��������
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Knight_Man/Mesh/SK_Knight_Man_black.SK_Knight_Man_Black'");
	GetMesh()->SetSkeletalMesh(mesh);
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
	//TODO: Lag�� ���� ���̴� ��� �����Ұ�
	SpringArm->CameraRotationLagSpeed = 2;
	SpringArm->bDoCollisionTest = false;	//�������Ͽ� �ɸ��� ���ư�����
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


	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACHuman::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACHuman::OnMoveRight);
	//���� ������ �Է¿��� ���ǵ� �̸��� �̺�Ʈ ���ε�
	//����ȭ ���� �ʾƵ� �ȴ� �Լ� ������ ���ε�
	//BindAxis�� ���ڷ� float �ϳ��� �޾ƾ� �Ѵ�

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACHuman::HorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACHuman::VerticalLook);
}

void ACHuman::OnJumpPressed()
{
	Super::Jump();
}

void ACHuman::OnJumpReleased()
{
	Super::StopJumping();
}

void ACHuman::OnMoveForward(float const InAxisValue)
{
	//�����̸� InAxisValue 1, �Ĺ��̸� -1

	FRotator const Rotator = FRotator(0, GetControlRotation().Yaw, 0);
	//��Ʈ�ѷ��� �ٶ󺸴� ���� ���� Yaw ���� ������
	FVector const Direction = FQuat(Rotator).GetForwardVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�
	//Rotator(ī�޶� �ٶ󺸴� Yaw)�� Q(X,Y,Z) , GetForwardVector()�� �����Ͽ� ���̸� ���λ���

	AddMovementInput(Direction, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const Rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const Direction = FQuat(Rotator).GetRightVector().GetSafeNormal2D();//������ ���������� ��ֶ���� �ʿ�

	AddMovementInput(Direction, InAxisValue);
}

void ACHuman::HorizontalLook(float const InAxisValue)
{
	AddControllerYawInput(InAxisValue);
}

void ACHuman::VerticalLook(float const InAxisValue)
{
	AddControllerPitchInput(InAxisValue);
}
