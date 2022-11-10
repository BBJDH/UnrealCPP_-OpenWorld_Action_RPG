#include "Weapon/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundWave.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOwner);
	if (!!state)
		state->SetActionMode();


	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRatio);
}

///////////////////////////////////////////////////////////////////////////////

void FHitData::PlayMontage(ACharacter* InOwner)
{
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRatio);
}

void FHitData::SendDamage(ACharacter* InAttacker, ACharacter* InOther)
{
	FHitDamageEvent e;
	e.HitData = this;

	InOther->TakeDamage(Power, e, InAttacker->GetController(), nullptr);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}


	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda
	(
		[=]()
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1.0f;
		}
	);

	
	FTimerHandle handle;
	InWorld->GetTimerManager().SetTimer(handle, timerDelegate, StopTime, false);
}

void FHitData::PlaySound(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(EffectLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}