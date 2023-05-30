

#include "Weapon/CAttachment.h"

#include "GameFramework/Character.h"
#include "Global.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	//CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");

	Root = CreateDefaultSubobject<USceneComponent>("Root");

	CHECK_NULL_UOBJECT(Root);

	this->SetRootComponent(Root);
}

void ACAttachment::OnBeginEquip()
{
	AttachTo(EquipSocketName);
}

void ACAttachment::OnUnequip()
{
	AttachTo(UnEquipSocketName);
}

void ACAttachment::BeginPlay()
{
	//와!!!!!!
	UE_LOG(GameProject, Warning, TEXT("FunctionCall : %s, LINE : %s"), *FString(__FUNCTION__), *FString::FromInt(__LINE__));
	Super::BeginPlay();
	UE_LOG(GameProject, Warning, TEXT("FunctionCall : %s, LINE : %s"), *FString(__FUNCTION__), *FString::FromInt(__LINE__));

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//모든 충돌체에 충돌 함수를 바인드
	GetComponents<UShapeComponent>(Collisions);
	for (UShapeComponent* component : Collisions)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		//미사용
		//component->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}
	//AttachTo(UnEquipSocketName);
	OffCollision();


}

//모든 충돌체 충돌 On
void ACAttachment::OnCollision()
{
	if (OnAttachmentCollision.IsBound())
		OnAttachmentCollision.Broadcast();

	for (UShapeComponent* component : Collisions)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAttachment::OffCollision()
{
	if (OffAttachmentCollision.IsBound())
		OffAttachmentCollision.Broadcast();

	for (UShapeComponent* component : Collisions)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACAttachment::OnAirCollision()
{
	if (OnAttachmentCollision.IsBound())
		OnAttachmentCollision.Broadcast();

	for (UShapeComponent* component : Collisions)
	{
		component->SetRelativeScale3D(FVector(1, 2, 1));
		//component->SetWorldScale3D(FVector(1, 2, 1));
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAttachment::OffAirCollision()
{
	if (OffAttachmentCollision.IsBound())
		OffAttachmentCollision.Broadcast();

	for (UShapeComponent* component : Collisions)
	{
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//component->SetWorldScale3D(FVector(1, 1, 1));
		component->SetRelativeScale3D(FVector(1, 1, 1));

	}
}

void ACAttachment::AttachTo(FName InSocketName)
{
	UE_LOG(GameProject, Warning, TEXT("FunctionCall : %s, LINE : %s"), *FString(__FUNCTION__), *FString::FromInt(__LINE__));

	CHECK_NULL_UOBJECT(OwnerCharacter);
	CHECK_NULL_UOBJECT(OwnerCharacter->GetMesh());
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	CHECK_TRUE(OwnerCharacter == OtherActor);

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, Cast<UShapeComponent>(OverlappedComponent), Cast<ACharacter>(OtherActor));
}

//미사용
// void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	CHECK_TRUE(OwnerCharacter == OtherActor);
//
// 	if (OnAttachmentEndOverlap.IsBound())
// 		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<UShapeComponent>(OverlappedComponent), Cast<ACharacter>(OtherActor));
// }
//
