// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TPS_RPG_API UCUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void SetNameText(FString const& InName);

	UFUNCTION(BlueprintImplementableEvent)
		void SetControllerNameText(FString const& InName);
	
};
