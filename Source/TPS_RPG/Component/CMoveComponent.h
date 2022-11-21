#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMoveComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_RPG_API UCMoveComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 200, 400, 600 };

	UPROPERTY(EditAnywhere, Category = "Mouse")
		FVector2D MouseSpeed = FVector2D(45, 45);

public:
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

public:	
	UCMoveComponent();

protected:
	virtual void BeginPlay() override;

public:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

public:
	void SetSpeed(ESpeedType InType);

	void OnSprint();
	void OffSprint();

public:
	void EnableControlRotation();
	void DisableControlRotation();

private:
	class ACharacter* Owner;

private:
	bool bCanMove = true;
	bool bFixedCamera;
};
