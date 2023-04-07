#include "Weapon/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Animation/AnimMontage.h"
#include "Niagara/Classes/NiagaraSystem.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOwner);
	if (state != nullptr)
		state->SetActionMode();

	if (Montage != nullptr)
		InOwner->PlayAnimMontage(Montage, PlayRatio);
}

///////////////////////////////////////////////////////////////////////////////

void FHitData::PlayMontage(ACharacter* InOwner)
{
	if (Montage != nullptr)
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

		if (character != nullptr)
		{
			character->CustomTimeDilation = 5e-2f;

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
	CheckNullUObject(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNullUObject(Effect);

	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(EffectLocation);
	
	CHelpers::PlayNiagaraEffect(InWorld, Cast<UNiagaraSystem>(Effect), transform);
	CHelpers::PlayParticleEffect(InWorld, Cast<UParticleSystem>(Effect) , transform);
}

void FHitData::HitLaunch(ACharacter* InOwner, FRotator const& InLookAtRotator , ACharacter * InAttacker)
{
	CheckNullUObject(InOwner);

	//FVector const LocationOfSelf = InOwner->GetActorLocation();
	//FVector const LocationOfAttacker = InAttacker->GetActorLocation();
	//FVector LookAtAttacker = LocationOfAttacker - LocationOfSelf;
	//LookAtAttacker.Z = 0;
	
	FVector LaunchDirection = FQuat(InLookAtRotator + this->LaunchRotation).GetForwardVector();
	LaunchDirection.Normalize();

	InOwner->LaunchCharacter(LaunchDirection * this->Launch, false, true);

	CheckNullUObject(InAttacker);
	if (this->IsLaunchAttacker)
	{
		InAttacker->LaunchCharacter(LaunchDirection * this->Launch * 1.02f, false, true);
	}

}
