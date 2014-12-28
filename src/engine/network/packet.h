#ifndef _PACKET_H
#define _PACKET_H

#include "network.h"
#include "../common/list.h"

class CNetMessage;
class CNetPacket
{
public:
	CNetPacket();
	~CNetPacket();

	CNetMessage* CreateMessage(int guid, MESSAGE_TYPES type);

protected:
	CLinkedList<CNetMessage> mMessages;
};

#endif
