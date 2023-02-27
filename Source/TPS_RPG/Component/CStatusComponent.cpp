#include "Component/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent():InAir(false)
{

}

void UCStatusComponent::CheckFarFromGround()
{
	FVector const traceStart = OwnerCharacter->GetActorLocation();
	FVector const traceEnd = { traceStart.X,traceStart.Y,traceStart.Z - Falling_Distance };
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);
	FHitResult HitResult{};

	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		traceStart,
		traceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ignoreActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (HitResult.bBlockingHit)
		InAir =false;
	InAir = true;
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Health = MaxHealth;

	//CheckNull(OwnerCharacter);
	//Cast<ACHuman>(OwnerCharacter)->StartFall.AddUFunction(this, "StartInAir");
	//Cast<ACHuman>(OwnerCharacter)->EndFall.AddUFunction(this, "EndInAir");
}

void UCStatusComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}
