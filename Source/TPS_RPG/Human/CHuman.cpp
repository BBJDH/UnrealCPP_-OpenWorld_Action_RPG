

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

	//Pawn Yaw를 컨트롤러에 귀속시키지 않음
	this->bUseControllerRotationYaw = false;
	//캐릭터무브먼트에 지정된 가속만큼 이동방향으로 회전방향을 잡아줌
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MAX_WALK_SPEED;

	USkeletalMesh* mesh = nullptr;

	//레퍼런스로 에셋 가져오기
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Knight_Man/Mesh/SK_Knight_Man_black.SK_Knight_Man_Black'");
	GetMesh()->SetSkeletalMesh(mesh);
	//위치 조정, 아래로 90, 
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
	SpringArm->bEnableCameraLag = true;		//카메라 부드럽게 따라오도록
	//TODO: Lag을 세게 먹이는 방법 연구할것
	SpringArm->CameraRotationLagSpeed = 2;
	SpringArm->bDoCollisionTest = false;	//스프링암에 걸리면 돌아가도록
	SpringArm->bUsePawnControlRotation = true;	//Pawn따라서 회전할지

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
	//엔진 세팅의 입력에서 정의된 이름의 이벤트 바인딩
	//직렬화 하지 않아도 된다 함수 포인터 바인딩
	//BindAxis는 인자로 float 하나를 받아야 한다

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
	//전방이면 InAxisValue 1, 후방이면 -1

	FRotator const Rotator = FRotator(0, GetControlRotation().Yaw, 0);
	//컨트롤러가 바라보는 전방 벡터 Yaw 값을 가져옴
	FVector const Direction = FQuat(Rotator).GetForwardVector().GetSafeNormal2D();//포워드 벡터이지만 노멀라이즈가 필요
	//Rotator(카메라가 바라보는 Yaw)의 Q(X,Y,Z) , GetForwardVector()는 외적하여 길이를 늘인상태

	AddMovementInput(Direction, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const Rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const Direction = FQuat(Rotator).GetRightVector().GetSafeNormal2D();//포워드 벡터이지만 노멀라이즈가 필요

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
