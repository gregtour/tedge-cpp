#include "networkObj.h"
#include "../common/list.h"

extern CLinkedList<INetworkObj> gNetworkObjList;

INetworkObj::INetworkObj()
{
	gNetworkObjList.Add(this);
}

INetworkObj::~INetworkObj()
{
	gNetworkObjList.RemoveEntry(gNetworkObjList.FindEntry(this));
}

void INetworkObj::NetSend()
{
}

void INetworkObj::NetReceive()
{
}
