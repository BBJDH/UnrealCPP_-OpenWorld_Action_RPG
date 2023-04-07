

#include "Weapon/CAttachment.h"

#include "GameFramework/Character.h"
#include "Global.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	//CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");

	Root = CreateDefaultSubobject<USceneComponent>("Root");

	CheckNullUObject(Root);

	this->SetRootComponent(Root);
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	GetComponents<UShapeComponent>(Collisions);
	for (UShapeComponent* component : Collisions)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}

	OffCollision();

	Super::BeginPlay();
}


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
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	CheckTrue(OwnerCharacter == OtherActor);

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, Cast<UShapeComponent>(OverlappedComponent), Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<UShapeComponent>(OverlappedComponent), Cast<ACharacter>(OtherActor));
}

