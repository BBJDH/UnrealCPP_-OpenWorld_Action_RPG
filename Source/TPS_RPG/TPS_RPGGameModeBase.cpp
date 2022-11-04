

#include "TPS_RPGGameModeBase.h"
#include "Global.h"

ATPS_RPGGameModeBase::ATPS_RPGGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/BP/Human/BP_CHuman.BP_CHuman_C'");
}