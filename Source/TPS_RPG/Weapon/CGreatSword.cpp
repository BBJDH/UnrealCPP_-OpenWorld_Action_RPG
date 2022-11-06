// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CGreatSword.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Human/CHuman.h"

ACGreatSword::ACGreatSword()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule",Mesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'");
	Mesh->SetSkeletalMesh(mesh);


	Capsule->SetRelativeLocation(FVector(0,0,80));
	Capsule->SetCapsuleHalfHeight(90);
	Capsule->SetCapsuleRadius(40);

	Mesh->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
}


void ACGreatSword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACGreatSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACGreatSword::Equip()
{
}

void ACGreatSword::Begin_Equip()
{
}

void ACGreatSword::End_Equip()
{
}

void ACGreatSword::UnEquip()
{
}

void ACGreatSword::Begin_UnEquip()
{
}

void ACGreatSword::End_UnEquip()
{
}

