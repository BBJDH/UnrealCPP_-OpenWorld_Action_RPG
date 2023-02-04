#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#define CheckTrue(p) {if(p==true) return;} // True�� �Լ� ����
#define CheckTrueResult(p,result) {if(p==true) return result;}// True�� �Լ� ����, ������

#define CheckFalse(p) {if(p==false) return;}// false�� �Լ� ����
#define CheckFalseResult(p,result) {if(p==false) return result;} // false�� �Լ� ����, ������
//True�� ���� ������ �ִٸ� �� ��ȯ

#define CheckNull(p) {if(p==nullptr) return;}	//nullptr�̶�� �ش� �Լ� ����
#define CheckNullResult(p,result) {if(p==nullptr) return result;}// nullptr�̶�� ���ڰ�p�� ����


#define CreateTextRender() \
{ \
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root); \
	Text->SetRelativeLocation(FVector(0, 0, 100)); \
	Text->SetRelativeRotation(FRotator(0, 180, 0)); \
	Text->SetRelativeScale3D(FVector(2)); \
	Text->TextRenderColor = FColor::Red; \
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center; \
	Text->Text = FText::FromString(GetName().Replace(TEXT("Default__"), TEXT(""))); \
}

#define and &&
#define or ||
#define not !


//�𸮾� �ʿ��� �⺻���� ��ҵ� ���

// �� ������Ʈ�� ����̸��� �������� �ñ��ϴٸ�
//.uproject ������
/*

	"Modules": [
		{
			"Name": "CPP_Basic",
			"Type": "Runtime",
			"LoadingPhase": "Default",
			"AdditionalDependencies": [
				"Engine"
			]
*///�� Ȯ��

//�� Ŭ������ ��� ��⿡ ���ԵǾ� �ִ��� : CPP_BASIC_API
class TPS_RPG_API CHelpers
{

public:
	//
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent,
		FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	{//��� Actor�� �Ҽӵ���/��𿡼� ��������(InActor), �Ҵ���� ������Ʈ ������, �����̸�, �޶���� �θ�(���� ����)		
		//Scene ������Ʈ ���� Ʈ�������� ����� ���⼭���� �θ��ڽ� ��İ���(��ǥ)�� �����ǹǷ� �ֻ����� USceneComponent�� ���

		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

			if (!!InParent)		//nullptr üũ
			{
				//�θ� SceneComponent�� �����Ѵٸ�
				(*OutComponent)->SetupAttachment(InParent, InSocketName);
				//���δ� ��İ� �ǽ�

				return;
			}
		InActor->SetRootComponent(*OutComponent);
		//InActor->RootComponent(*OutComponent); //Protected

	}


	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		//CreateComponent �Լ� ����
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
		//GetComponents�� �ش������ ��� ������Ʈ���� �����´�
		//���� ������Ʈ, ���ϴ� Ŭ������ ���������� ���ø� �ۼ�
	}

	template<typename T>
	static T* GetComponent(AActor* InActor, const FString & InName)
	{
		TArray<T *> components;

		InActor->GetComponens<T>(components);

		for (T* COMPONENT : components)
		{
			if (component->GetName() == InName)
				return COMPONENT;
		}
		
		return nullptr;
		//GetComponents�� �ش������ ��� ������Ʈ���� �����´�
		//���� ������Ʈ, ���ϴ� Ŭ������ ���������� ���ø� �ۼ�
	}


	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		/*************************************
		Cast<Typename>(); �ش� Ÿ������ ĳ����
		**************************************/
		*OutObject = Cast<T>
			(
				StaticLoadObject
				(
					T::StaticClass(),
					nullptr,
					*InPath
				)
			);
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class; 
	}

	template<typename T>
	static T* FindActor(UWorld * InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)//������ ���� ������ ��� ���� ��ȸ
		{
			if (!!actor && actor->IsA<T>()) //IsA ����̴� : TRUE
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

		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)//������ ���� ������ ��� ���� ��ȸ
		{
			if (!!actor && actor->IsA<T>()) //IsA ����̴� : TRUE
			{
				OutArray.Add(Cast<T>(actor));
			}
		}
//		return nullptr;
	}


	static void PlayParticleEffect(UWorld* InWorld, UParticleSystem* InEffect, const FTransform& InTransform, USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None)
	{
		CheckNull(InEffect);
		
		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (!!InMesh)
		{
			if (!!InEffect)
			{
				UGameplayStatics::SpawnEmitterAttached(InEffect, InMesh, InSocketName, location, rotation, scale);

				return;
			}
		}

		if (!!InEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld, InEffect, InTransform);

			return;
		}
	}
	static void PlayNiagaraEffect(UWorld* InWorld, UNiagaraSystem* InEffect, const FTransform& InTransform, USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None)
	{
		CheckNull(InEffect);
		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (!!InMesh)
		{
			if (!!InEffect)
			{
				//UGameplayStatics::SpawnEmitterAttached(InEffect, InMesh, InSocketName, location, rotation, scale);
				UNiagaraFunctionLibrary::SpawnSystemAttached(InEffect, InMesh, InSocketName, location, rotation, EAttachLocation::SnapToTarget, true);
				return;
			}
		}

		if (!!InEffect)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(InWorld, InEffect, InTransform);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, InEffect, InTransform.GetLocation(), InTransform.Rotator(), InTransform.GetScale3D());
			return;
		}
	}
};