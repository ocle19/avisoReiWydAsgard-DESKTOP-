#include "CClient.h"
#include "CSocket.h"
#include "GetFunc.h"
#include "SendFunc.h"

// constructor
CClient::CClient()
{
	SelectedMob = -1;
	Token = false;
	SecCounter = 0;
	Session = 0;

	Water.Moving = 0;
	Water.Room = 0;
	Water.Status = 0;
	Water.Type = 0;

	Macro.Type = 0;
	Macro.BarIndex = -1;
	Macro.CurrentId = -1;
	Macro.SkillId = -1;
	Macro.OutOfPosition = 0;

	memset(Party, 0, sizeof Party);
	memset(Players, 0, sizeof Players);
	memset(Storage, 0, sizeof st_Item * 127);
	memset(&EnemyList, 0, sizeof EnemyList);
}

CClient::~CClient()
{
}
