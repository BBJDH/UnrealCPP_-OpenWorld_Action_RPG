// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/CANS_AirborneLeap.h"
#include "GameFramework/PlayerController.h"
#include "Global.h"
#include "Human/CHuman.h"

FString UCANS_AirborneLeap::GetNotifyName_Implementation() const
{
	return "AirborneLeap";
}

void UCANS_AirborneLeap::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	IsKeyPressed = false;
	CheckNullUObject(MeshComp);
	CheckNullUObject(MeshComp->GetOwner());

	Player = Cast<ACHuman>(MeshComp->GetOwner());
	CheckNullUObject(Player);
	CheckNullUObject(Player->GetController());
	PlayerController =Cast<APlayerController>(Player->GetController());

}

void UCANS_AirborneLeap::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	CheckNullUObject(PlayerController);
	if (PlayerController->IsInputKeyDown(EKeys::E))
		IsKeyPressed = true;
}

void UCANS_AirborneLeap::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNullUObject(Player);
	if (IsKeyPressed)
	{
		Player->LaunchCharacter(FVector(0, 0, 1200), false, false);
		//if (Player->StartFall.IsBound())
		//	Player->StartFall.Broadcast();
	}
}
