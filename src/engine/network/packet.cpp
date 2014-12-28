#include "packet.h"
#include "message.h"

CNetPacket::CNetPacket()
{

}

CNetPacket::~CNetPacket()
{

}

CNetMessage* CNetPacket::CreateMessage(int guid, MESSAGE_TYPES type)
{
	CNetMessage* msg = new CNetMessage(guid, type);
	mMessages.Add(msg);
	return msg;
}
