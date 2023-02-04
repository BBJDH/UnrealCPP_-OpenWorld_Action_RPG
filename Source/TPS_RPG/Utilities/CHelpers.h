#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#define CheckTrue(p) {if(p==true) return;} // True시 함수 종료
#define CheckTrueResult(p,result) {if(p==true) return result;}// True시 함수 종료, 값리턴

#define CheckFalse(p) {if(p==false) return;}// false시 함수 종료
#define CheckFalseResult(p,result) {if(p==false) return result;} // false시 함수 종료, 값리턴
//True시 종료 리턴이 있다면 값 반환

#define CheckNull(p) {if(p==nullptr) return;}	//nullptr이라면 해당 함수 종료
#define CheckNullResult(p,result) {if(p==nullptr) return result;}// nullptr이라면 인자값p를 리턴


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


//언리얼에 필요한 기본적인 요소들 헤더

// 내 프로젝트의 모듈이름이 무엇인지 궁금하다면
//.uproject 파일의
/*

	"Modules": [
		{
			"Name": "CPP_Basic",
			"Type": "Runtime",
			"LoadingPhase": "Default",
			"AdditionalDependencies": [
				"Engine"
			]
*///를 확인

//이 클래스가 어느 모듈에 포함되어 있는지 : CPP_BASIC_API
class TPS_RPG_API CHelpers
{

public:
	//
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent,
		FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	{//어느 Actor에 소속될지/어디에서 생성될지(InActor), 할당받을 컴포넌트 포인터, 생성이름, 달라붙을 부모(유무 포함)		
		//Scene 컴포넌트 부터 트랜스폼이 생긴다 여기서부터 부모자식 행렬관계(좌표)가 형성되므로 최상위인 USceneComponent를 사용

		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

			if (!!InParent)		//nullptr 체크
			{
				//부모 SceneComponent가 존재한다면
				(*OutComponent)->SetupAttachment(InParent, InSocketName);
				//붙인다 행렬곱 실시

				return;
			}
		InActor->SetRootComponent(*OutComponent);
		//InActor->RootComponent(*OutComponent); //Protected

	}


	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		//CreateComponent 함수 참고
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
		//GetComponents는 해당액터의 모든 컴포넌트들을 가져온다
		//단일 컴포넌트, 원하는 클래스로 가져오도록 템플릿 작성
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
		//GetComponents는 해당액터의 모든 컴포넌트들을 가져온다
		//단일 컴포넌트, 원하는 클래스로 가져오도록 템플릿 작성
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
		Cast<Typename>(); 해당 타입으로 캐스팅
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
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)//월드의 현재 레벨의 모든 액터 순회
		{
			if (!!actor && actor->IsA<T>()) //IsA 상속이다 : TRUE
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

		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)//월드의 현재 레벨의 모든 액터 순회
		{
			if (!!actor && actor->IsA<T>()) //IsA 상속이다 : TRUE
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