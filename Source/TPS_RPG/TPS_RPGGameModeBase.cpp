

#include "TPS_RPGGameModeBase.h"
#include "Global.h"

ATPS_RPGGameModeBase::ATPS_RPGGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/BP/Human/Player/BP_CHuman_Player.BP_CHuman_Player_C'");
}