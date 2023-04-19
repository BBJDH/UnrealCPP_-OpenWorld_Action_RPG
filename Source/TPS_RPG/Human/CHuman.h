
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CHuman.generated.h"

DECLARE_MULTICAST_DELEGATE(FActionCall);


UCLASS()
class TPS_RPG_API ACHuman
: public ACharacter
, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACHuman();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return static_cast<FGenericTeamId>(this->TeamID);}



	//CallByNotify
	void NotifyDead();

protected:

	virtual void Landed(const FHitResult& Hit) override;
	virtual void Falling() override;

	//BindAction
	void OnJumpPressed();
	void OnJumpReleased();
	//void OnDodgeAction() const;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType const InPrevType, EStateType InNewType);

	//Broadcast InAir
	void StartFall() const;
	void EndFall() const;

	void GetHit();
	void Dead()const;

	FVector GetVectorLookAtActor(AActor const* InActor) const;
	void SetActorRotation2D(FRotator LookAtRotator);


protected:
	//Components
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//Custom 
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMoveComponent* Move;


private:
	struct FDamageData
	{
		float Amount;
		class ACharacter* Attacker;
		struct FHitDamageEvent* Event;
	};
	FDamageData DamageData;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

	int CustomJumpCount = 0;

};



