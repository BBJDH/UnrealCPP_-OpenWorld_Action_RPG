
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttachmentCollision);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAttachmentOverlap, class ACharacter*, class UShapeComponent*,class ACharacter*);

UCLASS()
class TPS_RPG_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

	//UFUNCTION(BlueprintImplementableEvent)
	void OnBeginEquip();

	//UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();

	void OnCollision();
	void OffCollision();

	void OnAirCollision();
	void OffAirCollision();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Attachment")
		void AttachTo(FName InSocketName);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

	// UFUNCTION()
	// 	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;


	
public:
	FAttachmentOverlap OnAttachmentBeginOverlap;
	FAttachmentOverlap OnAttachmentEndOverlap;

	FAttachmentCollision OnAttachmentCollision;
	FAttachmentCollision OffAttachmentCollision;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	TArray<class UShapeComponent*> Collisions;

private:
	UPROPERTY(EditAnywhere,Category = "SoketName")
	FName UnEquipSocketName;

	UPROPERTY(EditAnywhere,Category = "SoketName")
	FName EquipSocketName;
};
