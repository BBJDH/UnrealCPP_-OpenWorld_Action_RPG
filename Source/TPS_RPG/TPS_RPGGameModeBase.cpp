

#include "TPS_RPGGameModeBase.h"
#include "Global.h"

ATPS_RPGGameModeBase::ATPS_RPGGameModeBase()
{
	//³²Ä³ - Blueprint'/Game/BP/Human/Player/BP_CHuman_Player.BP_CHuman_Player'
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/BP/Human/Player/Satomi/BP_CHuman_Satomi.BP_CHuman_Satomi_C'");
}