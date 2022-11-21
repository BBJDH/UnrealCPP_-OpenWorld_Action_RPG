#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS( ClassGroup=(Custom) )
class TPS_RPG_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();
	void Damage(float InAmount);
	FORCEINLINE float GetHealth() const { return Health; }

	FORCEINLINE bool IsInAir() const { return InAir; }
	void CheckFarFromGround();

protected:
	virtual void BeginPlay() override;


private:
	UFUNCTION()
		void StartInAir(){ InAir = true; }
	UFUNCTION()
		void EndInAir(){ InAir = false; }

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;

	float const MAX_Ground_Far = 400;

private:
	class ACharacter* OwnerCharacter;
	float Health;
	bool InAir;
};
