#ifndef _NETWORKOBJ_H
#define _NETWORKOBJ_H

#include "../network/network.h"

enum
{
	LOCAL_OWNER,
	REMOTE_OWNER
};

struct SNetVarBindings
{
	SNetVarBindings(const char* label, int labelLen, void* data, int size) 
		: label(label), labelLen(labelLen), data(data), size(size)
	{ return; }

	const char* label;
	int labelLen;
	void* data;
	int	size;
};

class INetworkObj
{
public:
	INetworkObj();
	virtual ~INetworkObj();

	virtual void NetSend();
	virtual void NetReceive();

protected:
	int	mOwnership;
	CLinkedList<SNetVarBindings> mAutoMaintainVariables;
};


#define AUTO_NET_ADD_VARIABLE(var) {							\
	mAutoMaintainVariables.Add(new SNetVarBindings(				\
		#var,													\
		sizeof(#var),											\
		(void*)(&var),											\
		sizeof(var)));											\
}

#define AUTO_NET_ADD_POINTER(ptr) {								\
	mAutoMaintainVariables.Add(new SNetVarBindings(				\
		#ptr,													\
		sizeof(#ptr),											\
		(void*)(ptr),											\
		sizeof(*ptr)));											\
}

#endif
