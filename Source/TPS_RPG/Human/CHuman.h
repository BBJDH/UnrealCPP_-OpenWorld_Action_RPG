
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "CHuman.generated.h"
/*
 ======================================

 ======================================
 */

DECLARE_MULTICAST_DELEGATE(FActionCall);


UCLASS()
class TPS_RPG_API ACHuman : public ACharacter
{
	GENERATED_BODY()

public:
	ACHuman();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void Asign();


	virtual void Landed(const FHitResult& Hit) override;
	virtual void Falling() override;


	//BindAction
	void OnJumpPressed();
	void OnJumpReleased();


	//BindAxis
	void OnMoveForward(float const InAxisValue);
	void OnMoveRight(float const InAxisValue);


private:


	UFUNCTION()
		void OnStateTypeChanged(EStateType const InPrevType, EStateType InNewType);


	/*
	 * 액션마다 고유의 피격 몽타주를 재생시키면 보스의 특수화를 어떻게 할까
	 *오버라이딩을 이용해서 구현한다면 몽땅 따로 특수화를 해줘야하는가
	 *
	 */
	void Hitted();
	void Dead();


public:
	FActionCall StartFall;
	FActionCall EndFall;


	//컴포넌트
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//커스텀 
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;


private:
	struct FDamageData
	{
		float Amount;
		class ACharacter* Attacker;
		struct FHitDamageEvent* Event;
	};
	FDamageData DamageData;


};



