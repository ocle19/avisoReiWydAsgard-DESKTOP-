#include "CSocket.h"
#include "GetFunc.h"
#include "Struct.h"

void SendAction(INT32 posX, INT32 posY, INT32 targetX, INT32 targetY, INT32 speed, INT32 type)
{
	p36C packet;
	memset(&packet, 0, sizeof packet);

	packet.Header.PacketId = 0x36C;
	packet.Header.Size     = sizeof packet;
	packet.Header.CheckSum = 0;

	packet.Destiny.X = targetX;
	packet.Destiny.Y = targetY;

	packet.LastPos.X = posX;
	packet.LastPos.Y = posY;

	packet.MoveSpeed = speed;
	packet.MoveType  = type;

	//cServer.AddMessage((char*)&packet, sizeof packet);
}

void SendRequestUseItem(INT32 invType, INT32 slotId, INT32 dstType, INT32 dstSlot)
{
	p373 packet;
	memset(&packet, 0, sizeof packet);

	packet.Header.PacketId = 0x373;
	packet.Header.Size     = sizeof packet;
	packet.Header.ClientId = 0;

	packet.DstSlot = dstSlot;
	packet.DstType = dstType;

	packet.SrcSlot = slotId;
	packet.SrcType = invType;

	///packet.PosX = Client.Position.X;
	///packet.PosY = Client.Position.Y;
	
	///cServer.AddMessage((char*)&packet, sizeof packet);
}