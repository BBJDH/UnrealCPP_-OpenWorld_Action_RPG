

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
	//AnimInstance, Status, FeetComponent 전파
	//if (EndFall.IsBound() /*and Status->IsInAir()*/)
	//	EndFall.Broadcast();
}

void ACHuman::Falling()
{
	Super::Falling();

	StartFall();
	//AnimInstance, Status, FeetComponent 전파... 더 늘어난다면 디버깅은...?
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
	//AnimInstance, Status, FeetComponent 전파
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
	//전방이면 InAxisValue 1, 후방이면 -1

	FRotator const RotaterOfView = FRotator(0, GetControlRotation().Yaw, 0);
	//컨트롤러가 바라보는 전방 벡터 Yaw 값을 가져옴
	FVector const DirectionOfForward = FQuat(RotaterOfView).GetForwardVector().GetSafeNormal2D();//포워드 벡터이지만 노멀라이즈가 필요
	//Rotator(카메라가 바라보는 Yaw)의 Q(X,Y,Z) , GetForwardVector()는 외적하여 길이를 늘인상태

	//CLog::Print(static_cast<int>(SpringArm->bUsePawnControlRotation));
	AddMovementInput(DirectionOfForward, InAxisValue);
}

void ACHuman::OnMoveRight(float const InAxisValue)
{
	FRotator const RotaterOfView = FRotator(0, GetControlRotation().Yaw, 0);
	FVector const DirectionORight = FQuat(RotaterOfView).GetRightVector().GetSafeNormal2D();//포워드 벡터이지만 노멀라이즈가 필요

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
	//AnimInstance, Status, FeetComponent 전파
	UAnimInstance * SelfAnimInstance =  this->GetMesh()->GetAnimInstance();
	CheckNull(SelfAnimInstance);
	Cast<UCAnimInstance>(SelfAnimInstance)->StartInAir();
	Status->StartInAir();
	Feet->StartInAir();
	Weapon->InitComboIndex();

}

void ACHuman::EndFall()
{
	//AnimInstance, Status, FeetComponent 전파
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

	//Pawn Yaw를 컨트롤러에 귀속시키지 않음
	this->bUseControllerRotationYaw = false;
	//캐릭터무브먼트에 지정된 가속만큼 이동방향으로 회전방향을 잡아줌
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;

	USkeletalMesh* mesh = nullptr;

	//레퍼런스로 에셋 가져오기
	//CHelpers::GetAsset<USkeletalMesh>(&mesh, "");
	//GetMesh()->SetSkeletalMesh(mesh);
	//위치 조정, 아래로 90, 
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); //Pitch Yaw Roll

	TSubclassOf<UCAnimInstance> ClassOfAnimInstance;
	CHelpers::GetClass<UCAnimInstance>(&ClassOfAnimInstance, "AnimBlueprint'/Game/BP/Human/ABP_CHuman.ABP_CHuman_C'");

	GetMesh()->SetAnimClass(ClassOfAnimInstance);
	//위의 함수는 내부적으로 이함수를 호출 GetMesh()->SetAnimInstanceClass(animInstance);
}

