#ifndef _LOG_H
#define _LOG_H

#include <string>
#include "list.h"
#include "timer.h"

class CLogMessage
{
public:
	CLogMessage();
	~CLogMessage();
	CLogMessage( std::string t );

	std::string GetMessage();
	void SetTimeStamp( float t );
	float GetTimeStamp();
private:
	std::string	mText;
	float mTimeStamp;
};

class CLog
{
public:
	CLog();
	~CLog();
	void SetLogging( bool enabled );
	void LogItem( CLogMessage* message );
	void PrintLog();
	void SaveLog( std::string filename );
	void ClearLog();
	void ResetTimeStamp();

private:
	bool	mEnabled;
	CTimer* mTimeStamp;
	CLinkedList<CLogMessage>	mLogEntries;
};

#endif
