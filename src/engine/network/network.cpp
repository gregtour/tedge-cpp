/**
 * Network.cpp
**/
#include <stdio.h>
#include "SDL_net.h"

#include "../common/list.h"
#include "network.h"
#include "../entity/networkObj.h"

UDPsocket gSocket;
NETWORK_MODE gNetworkMode;
CLinkedList<INetworkObj>	gNetworkObjList;

int NetworkStartup()
{
#ifdef NETWORK_LOGGING	
	gLog.LogItem( new CLogMessage( "Network startup" ) );
#endif

	if (SDLNet_Init() < 0)
		return 0;

	gSocket = SDLNet_UDP_Open(NETWORK_PORT);



	return 1;
}

int NetworkShutdown()
{
	#ifdef NETWORK_LOGGING	
		gLog.LogItem( new CLogMessage( "Network shutdown" ) );
	#endif

	return 1;
}