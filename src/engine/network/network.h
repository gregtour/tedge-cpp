/**
 * Network.h
**/

#ifndef _NETWORK_H
#define _NETWORK_H

#include "SDL_net.h"
#include "../common/list.h"

#define NETWORK_PORT	101


int NetworkStartup();
int NetworkShutdown();

enum MESSAGE_TYPES
	{
	MESSAGE_CREATE,
	MESSAGE_UPDATE,
	MESSAGE_DESTROY
	};

enum NETWORK_MODE
	{
	NETWORK_CLIENT,
	NETWORK_SERVER,
	NETWORK_PEER
	};

class INetworkObj;

extern UDPsocket gSocket;
extern NETWORK_MODE gNetworkMode;
extern CLinkedList<INetworkObj>	gNetworkObjList;

#endif
