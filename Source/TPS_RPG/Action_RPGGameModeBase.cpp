

#include "Action_RPGGameModeBase.h"
#include "Global.h"

AAction_RPGGameModeBase::AAction_RPGGameModeBase()
{
	//��ĳ - Blueprint'/Game/BP/Human/Player/BP_CHuman_Player.BP_CHuman_Player'

	ConstructorHelpers::FClassFinder<APawn> AssetFound(*FString("Blueprint'/Game/BP/Human/Player/Satomi/BP_CHuman_Satomi.BP_CHuman_Satomi_C'"));
	CheckNullUObject(AssetFound.Class);
	DefaultPawnClass = AssetFound.Class;
	//CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/BP/Human/Player/Satomi/BP_CHuman_Satomi.BP_CHuman_Satomi_C'");
	//CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/BP/Human/Player/BP_CHuman_Player.BP_CHuman_Player_C'");
}