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

public:	
	UCMoveComponent();

	void OnMoveForward(float InAxis) ;
	void OnMoveRight(float InAxis) ;
	void OnHorizontalLook(float InAxis) const;
	void OnVerticalLook(float InAxis) const;

	void SetSpeed(ESpeedType InType);

	void OnSprint();
	void OffSprint();

	void EnableControlRotation();
	void DisableControlRotation();

	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float WalkSpeed[static_cast<int32>(ESpeedType::Max)] = { 200, 600, 1000 };

	UPROPERTY(EditAnywhere, Category = "Mouse")
		FVector2D SpeedOfMouseScroll = FVector2D(45, 45);

	UPROPERTY(VisibleDefaultsOnly)
	class ACharacter* OwnerCharacter;

	bool bCanMove = true;
	bool bFixedCamera;
};
