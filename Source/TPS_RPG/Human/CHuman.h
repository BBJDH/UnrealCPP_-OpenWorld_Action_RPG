
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
	void Tick(float DeltaTime) override;
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(this->TeamID);}

	//Broadcast Falling, Landed
	void StartFall();
	void EndFall();

	//CallByNotify
	void NotifyDead();

protected:

	virtual void Landed(const FHitResult& Hit) override;
	virtual void Falling() override;

	//BindAction
	void OnJumpPressed();
	void OnJumpReleased();

	//BindAxis
	//void OnMoveForward(float const InAxisValue);
	//void OnMoveRight(float const InAxisValue);

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType const InPrevType, EStateType InNewType);

	//Constructor Function
	void Asign();

	void GetHit();
	void Dead();

	FVector GetVectorLookAtActor(AActor const* InActor);
	void SetActorRotation2D(FRotator LookAtRotator);


	//ÄÄÆ÷³ÍÆ®
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//Ä¿½ºÅÒ 
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



