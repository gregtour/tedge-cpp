#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>

#include "network.h"

class CNetMessage
{
public:
	CNetMessage(int guid, MESSAGE_TYPES type);
	~CNetMessage();

	void AddData(std::string tag, void* data, int len);
	int GetLength();

protected:
	int mDataLen, mGuid;
	MESSAGE_TYPES mType;
	char* mData;
};

#endif
