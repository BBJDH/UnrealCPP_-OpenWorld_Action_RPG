

#include "Component/CMontageComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"



UCMontageComponent::UCMontageComponent()
{

}


void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(SettedMontageData);

	OwnerCharacter = Cast<ACharacter>(GetOwner());


	TArray<FMontageData*> LoadedDatas;
	SettedMontageData->GetAllRows("", LoadedDatas);

	for (int32 i = 0; i < static_cast<int32>(EMontageType::Max); i++)
	{
		for (FMontageData* elem : LoadedDatas)
		{
			if (static_cast<EMontageType>(i) == elem->Type)
				DatasOfPlayMontage[i] = elem;
		}
	}
}

void UCMontageComponent::PlayHitted() const
{
	PlayAnimMontage(EMontageType::Hitted);
}

void UCMontageComponent::PlayDead() const
{
	PlayAnimMontage(EMontageType::Dead);
}

void UCMontageComponent::PlayFirstJump() const
{
	PlayAnimMontage(EMontageType::Jump_First);

}

void UCMontageComponent::PlaySecondJump() const
{
	PlayAnimMontage(EMontageType::Jump_Second);
}

void UCMontageComponent::PlayLended() const
{
	PlayAnimMontage(EMontageType::Heavy_Landed);
}

void UCMontageComponent::PlayAnimMontage(EMontageType const InType) const
{
	CheckNull(OwnerCharacter);

	FMontageData* MontageDateToPlay = DatasOfPlayMontage[static_cast<int32>(InType)];
	if (MontageDateToPlay != nullptr && MontageDateToPlay->Montage != nullptr)
		OwnerCharacter->PlayAnimMontage(MontageDateToPlay->Montage, MontageDateToPlay->PlayRatio);
}


