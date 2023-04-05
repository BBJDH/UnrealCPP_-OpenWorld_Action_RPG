
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_RPG_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCZoomComponent();

	void SetZoomValue(float InValue);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void InterpCurrentZoomLength(float const DeltaTime);


private:
	UPROPERTY(EditAnywhere, Category = "Zooming")
		float ZoomSpeed = 20;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		float ZoomInterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		FVector2D ZoomRange = FVector2D(100, 800);

	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm;

	float DestValueOfZoomLength;
		
};
