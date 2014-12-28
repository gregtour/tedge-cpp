#include "message.h"

CNetMessage::CNetMessage(int guid, MESSAGE_TYPES type): mGuid(guid), mType(type)
{
}

CNetMessage::~CNetMessage()
{
}

void CNetMessage::AddData(std::string tag, void* data, int len)
{
	/*
		format is:
			[char (1 byte)] taglen
			[int (4 byte)] datalen
			[taglen] tag
			[datalen] data
		so total size is len+taglen+5
	*/
	int actualDataLen = len + tag.length() + 5;

	if(mDataLen==0)
	{
		mData = (char*) malloc(actualDataLen);
	} else {
		mData = (char*) realloc(mData, mDataLen + actualDataLen);
	}

	//copy data
	mData[mDataLen++] = tag.length();
	mData[mDataLen++] = len & 0xFF000000;
	mData[mDataLen++] = len & 0xFF0000;
	mData[mDataLen++] = len & 0xFF00;
	mData[mDataLen++] = len & 0xFF;

	memcpy((void*)mData[mDataLen], tag.data(), tag.length());
	mDataLen += tag.length();

	memcpy((void*)mData[mDataLen], data, len);
	mDataLen += len;
}

int CNetMessage::GetLength()
{
	return mDataLen;
}
