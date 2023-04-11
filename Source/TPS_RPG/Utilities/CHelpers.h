#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"//SpawnEmitter

DEFINE_LOG_CATEGORY_STATIC(GameProject, Display, All)
#define CheckTrue(Param) {if((Param)==true) return;} 
#define CheckTrueResult(Param,Result) {if((Param)==true) return Result;}

#define CheckFalse(Param) {if((Param)==false) return;}
#define CheckFalseResult(Param,Result) {if((Param)==false) return Result;} 

#define CheckNull(Param) {if((Param)==nullptr) return;}	
#define CheckNullResult(Param,Result) {if((Param)==nullptr) return Result;}

#define CheckNullUObject(Param) {if(IsValid(Param)==false) {UE_LOG(GameProject, Warning, TEXT("Null Or PendingKill, Function : %s, LINE : %s"), *FString(__FUNCTION__), *FString::FromInt(__LINE__));  return;}}	
#define CheckNullUObjectResult(Param,Result) {if(IsValid(Param)==false) {UE_LOG(GameProject, Warning, TEXT("Null Or PendingKill, Function : %s, LINE : %s"), *FString(__FUNCTION__), *FString::FromInt(__LINE__));  return Result;}}	

#define and &&
#define or ||
#define not !

//#define CreateTextRender() \
//{ \
//	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root); \
//	Text->SetRelativeLocation(FVector(0, 0, 100)); \
//	Text->SetRelativeRotation(FRotator(0, 180, 0)); \
//	Text->SetRelativeScale3D(FVector(2)); \
//	Text->TextRenderColor = FColor::Red; \
//	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center; \
//	Text->Text = FText::FromString(GetName().Replace(TEXT("Default__"), TEXT(""))); \
//}




class TPS_RPG_API CHelpers
{

public:
	//template<typename T>
	//static void CreateComponent(AActor* InActor, T** OutComponent,
	//	FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	//{
	//	*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

	//		if (InParent !=nullptr)		
	//		{
	//			(*OutComponent)->SetupAttachment(InParent, InSocketName);

	//			return;
	//		}
	//	InActor->SetRootComponent(*OutComponent);

	//}


	//template<typename T>
	//static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	//{
	//	*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	//}

	//template<typename T>
	//static T* GetComponent(AActor* InActor)
	//{
	//	return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	//}

	//template<typename T>
	//static T* GetComponent(AActor* InActor, const FString & InName)
	//{
	//	TArray<T *> components;

	//	InActor->GetComponens<T>(components);

	//	for (T* COMPONENT : components)
	//	{
	//		if (component->GetName() == InName)
	//			return COMPONENT;
	//	}
	//	
	//	return nullptr;
	//}


	//template<typename T>
	//static void GetAsset(T** OutObject, FString InPath)
	//{
	//	ConstructorHelpers::FObjectFinder<T> asset(*InPath);
	//	*OutObject = asset.Object;
	//}

	//template<typename T>
	//static void GetAssetDynamic(T** OutObject, FString InPath)
	//{
	//	*OutObject = Cast<T>
	//		(
	//			StaticLoadObject
	//			(
	//				T::StaticClass(),
	//				nullptr,
	//				*InPath
	//			)
	//		);
	//}

	//template<typename T>
	//static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	//{
	//	ConstructorHelpers::FClassFinder<T> asset(*InPath);
	//	*OutClass = asset.Class; 
	//}

	template<typename T>
	static T* FindActor(UWorld * InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (actor != nullptr && actor->IsA<T>())
			{
				return Cast<T>(actor);
			}
		}
		return nullptr;
	}

	template<typename T>
	static void FindActors(UWorld * InWorld, TArray<T *>& OutArray)
	{
		OutArray.Empty();

		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (actor != nullptr && actor->IsA<T>()) 
			{
				OutArray.Add(Cast<T>(actor));
			}
		}
	}


	static void PlayParticleEffect(UWorld* InWorld, UParticleSystem* InEffect, const FTransform& InTransform, USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None)
	{
		CheckNullUObject(InEffect);
		
		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (InMesh != nullptr)
		{
			if (InEffect != nullptr)
			{
				UGameplayStatics::SpawnEmitterAttached(InEffect, InMesh, InSocketName, location, rotation, scale);
				
				return;
			}
		}

		if (InEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld, InEffect, InTransform);

			return;
		}
	}
	static void PlayNiagaraEffect(UWorld* InWorld, UNiagaraSystem* InEffect, const FTransform& InTransform, USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None)
	{
		CheckNullUObject(InEffect);
		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (InMesh != nullptr)
		{
			if (InEffect != nullptr)
			{
				//UGameplayStatics::SpawnEmitterAttached(InEffect, InMesh, InSocketName, location, rotation, scale);
				UNiagaraFunctionLibrary::SpawnSystemAttached(InEffect, InMesh, InSocketName, location, rotation, EAttachLocation::SnapToTarget, true);
				return;
			}
		}

		if (InEffect != nullptr)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(InWorld, InEffect, InTransform);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, InEffect, InTransform.GetLocation(), InTransform.Rotator(), InTransform.GetScale3D());
			return;
		}
	}
};