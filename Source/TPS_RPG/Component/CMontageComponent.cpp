

#include "Component/CMontageComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"



UCMontageComponent::UCMontageComponent()
{

}


void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(MontageData);

	Owner = Cast<ACharacter>(GetOwner());


	TArray<FMontageData*> SubDatas;
	MontageData->GetAllRows("", SubDatas);

	for (int32 i = 0; i < static_cast<int32>(EMontageType::Max); i++)
	{
		for (FMontageData* Elem : SubDatas)
		{
			if (static_cast<EMontageType>(i) == Elem->Type)
				Datas[i] = Elem;
		}
	}
}

void UCMontageComponent::PlayHittedMode() const
{
	PlayAnimMontage(EMontageType::Hitted);
}

void UCMontageComponent::PlayDeadMode() const
{
	PlayAnimMontage(EMontageType::Hitted);
}

void UCMontageComponent::PlayAnimMontage(EMontageType const InType) const
{
	CheckNull(Owner);

	FMontageData* Data = Datas[static_cast<int32>(InType)];
	if (!!Data && !!Data->Montage)
		Owner->PlayAnimMontage(Data->Montage, Data->PlayRatio);
}


